import dash
from dash.dependencies import Output,Input
import dash_core_components as dcc
import dash_html_components as html
from random import random
import plotly

import rabbitmq_consumer

external_stylesheets = ['https://codepen.io/chriddyp/pen/bW6LwgP.css']

app = dash.Dash(__name__, external_stylesheets = external_stylesheets)
app.layout = html.Div(
    html.Div([
        html.H4('NYSE Live Feed'),
        dcc.Graph(id = 'live-update-graph-scatter', animate = True),
        dcc.Interval(
            id = 'interval-component',
            interval = 1000 #in milliseconds
        )
    ])
)

globalData = {
    'Symbol' : list(),
    'Price' : list(),
    'Time' : list()
}

def parse_message():
    msg = rabbitmq_consumer.consumer.receive().decode("utf-8")
    for datum in msg.split(","):
        field = datum.split(":")
        key = field[0]
        val = field[1]
        globalData[key].append(val)

        print("--------------------")
        print("\n msg received: {} \n".format(msg))
        print("--------------------")

@app.callback(Output('live-update-graph-scatter', 'figure'),
              [Input('interval-component', 'interval')])
def update_graph_scatter(n):
    traces = list()
    parse_message()
    
    for t in set(globalData['Symbol']):
        traces.append(
            plotly.graph_objs.Scatter(
                x = [val for idx, val in enumerate(globalData['Time'])  if globalData['Symbol'][idx] == t],
                y = [val for idx, val in enumerate(globalData['Price']) if globalData['Symbol'][idx] == t],
                name = 'Symbol {}'.format(t),
                mode = 'lines+markers'))

    return { 'data': traces }

if __name__ == '__main__':
    app.run_server()