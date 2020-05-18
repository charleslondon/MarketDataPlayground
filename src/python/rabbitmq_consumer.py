import sys
import pika
import numpy as np
import queue
import threading
import time

class RabbitMQ_Consumer(threading.Thread):
    def __init__(self, name, host='localhost', port=5672, exchange_name='TEST', exchange_type='direct', routing_key='key', prefetch=3):
        threading.Thread.__init__(self)
        self.name = name
        self.host = host
        self.port = port
        self.exchange_name = exchange_name
        self.exchange_type = exchange_type
        self.routing_key = routing_key
        self.prefetch = prefetch
        self.channel = None
        self.queue = queue.Queue(10)
        self.do_exit = False

    def receive_callback(self, ch, method, properties, body):
        self.queue.put(body)

    class EOT(Exception):
        def __init__(self, consumer_name):
            Exception.__init__(self, "EOT received by " + consumer_name) 

    def run(self): 
        try:
            parameters = pika.ConnectionParameters(host=self.host, port=self.port)
            connection = pika.BlockingConnection(parameters)
            self.channel = connection.channel()
            self.channel.exchange_declare(exchange=self.exchange_name, exchange_type=self.exchange_type)
            self.queue_name = self.channel.queue_declare("key", exclusive=False).method.queue
            self.channel.queue_bind(exchange=self.exchange_name, queue=self.queue_name, routing_key=self.routing_key)
            self.channel.basic_consume(on_message_callback=self.receive_callback, queue=self.queue_name, auto_ack=True)
            self.channel.basic_qos(prefetch_count=self.prefetch)
            self.channel.add_on_cancel_callback(self.cancel_callback)

            print('[%s] Binding with queue: %s' % (self.name, self.queue_name))
            print('[%s] routing key: %s' % (self.name, self.routing_key))
            print(' [%s] Waiting for messages...' % self.name)

            self.channel.start_consuming()

            print(" [%s] Closing down connections..." % self.name)      
            connection.close()
        except BaseException as e:
            print(" [%s] Exception has occurred! %s" % (self.name, e))
            
        print(" [%s] Finished" % self.name)

    def cancel_callback(self, method):
        print(" [%s] cancelling...", self.name)

    def receive(self):
        if self.is_alive() == False:
            raise Exception(' [%s] thread not running!' % self.name)

        return self.queue.get()

    def exit(self):
        if self.is_alive() == False:
            raise Exception(' [%s] thread not running!' % self.name)

        print(" [%s] Exiting..." % self.name)
        self.channel.stop_consuming()
        

if not __name__ == '__main__':
    consumer = RabbitMQ_Consumer('Consumer1', 'localhost', 5672, 'TEST', 'direct', 'key', 3)
    consumer.start()