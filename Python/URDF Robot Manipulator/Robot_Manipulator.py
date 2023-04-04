
from dash import Dash, html, Input, Output
import dash_bootstrap_components as dbc
from dash import dcc
import vtk
import dash_vtk
# Use helper to get a mesh structure that can be passed as-is to a Mesh
from dash_vtk.utils import to_mesh_state, presets
import pyvista as pv
import dash_daq as daq
import os
import webbrowser

daq_theme = {
    # Copied from Darkly DBC theme
    'primary':      '#375a7f',
    'secondary':    '#444444',
    'success':      '#00bc8c',
    'info':         '#3498db',
    'warning':      '#f39c12',
    'danger':       '#e74c3c',
    'light':        '#adb5bd',
    'dark':         '#303030',
    #daq specific
    'detail':       '#375a7f',
}
knob_style = {
    'fill':         daq_theme['light']
}

# -----------------------------------------------------------------------------
# GUI setup
# -----------------------------------------------------------------------------
app = Dash(__name__, external_stylesheets = [dbc.themes.DARKLY])    # Dash setup
server = app.server

IN2MM = 25.4 # inches to mm conversion, VTK and PyVista are in mm

# -----------------------------------------------------------------------------
# Populate the 3D view screen
# -----------------------------------------------------------------------------
root        =   "Assets/"
fileType    =   ".stl"
file_arr    =   ["J0", "J1"] # array of file names
# NOTE: the number of origins, positions, orientations, and colors must match the number of files being used
                #    J0           J1
origin_arr  =   [(0*IN2MM, 0*IN2MM, 0*IN2MM), (0*IN2MM, 0*IN2MM, 0*IN2MM)]
pos_arr     =   [(0*IN2MM, 0*IN2MM, 0*IN2MM), (0*IN2MM, 0*IN2MM, 0*IN2MM)] # array of positions (inches)
orn_arr     =   [(0, 0, 0), (0, 0, 0)]  # array of orientations, stored as angles
color_arr   =   [(0, 0, 1), (1, 0, 0)]  # array of colors for each component, values are 0-255/255 for maximum color intensity
geometry    =   []                      # array to hold geometry representations

# Create the geometry representation
for i in range(0, len(file_arr)):
    mesh = to_mesh_state(pv.read(root+file_arr[i]+fileType))
    child = dash_vtk.GeometryRepresentation(
        children = [
            dash_vtk.Mesh(state=mesh)
        ],
        actor = {
            "orientation": orn_arr[i],
            "position": pos_arr[i],
            "origin": origin_arr[i],
        },
        property = {
            "color": color_arr[i]
        },
    )
    geometry.append(child)

# -----------------------------------------------------------------------------
# Setup the over-arching page components
# -----------------------------------------------------------------------------    
robotViewer = dash_vtk.View(
    background = [34/255, 34/255, 34/255],
    children = [ # add the previously parsed VTK geometry to the viewe
        geometry[0],
        geometry[1],
    ], 
)

arm_controller = dbc.Card([
    dbc.Row([
        dbc.Col([
            html.H3('Axis 1'),
            daq.Knob(
                id='servo-knob1', 
                max = 180, 
                value = 90, 
                scale = {'interval':15},
                #theme = daq_theme,
                style = knob_style
            ),
            html.Div(id='knob-result1')  # set the ID that will be returned from the callback
        ]),
        dbc.Col([
            html.H3('Axis 2'),
            daq.Knob(
                id='servo-knob2', 
                max = 180, 
                value = 90, 
                scale = {'interval':15},
                color = daq_theme['primary']
            ),
            html.Div(id='knob-result2'), # set the ID that will be returned from the callback
        ]),
        dbc.Col([
            html.H3('Axis 2'),
            daq.Knob(
                id='servo-knob3', 
                max = 180, 
                value = 90, 
                scale = {'interval':15},
                color = daq_theme['primary']
            ),
            html.Div(id='knob-result3'), # set the ID that will be returned from the callback
        ])
    ])
])

# -----------------------------------------------------------------------------
# Define the callbacks for components with outputs
# ----------------------------------------------------------------------------- 
@app.callback(Output('knob-result1', 'children'), Input('servo-knob1', 'value')) # callback for updating data via knob interaction
def update_knob1(value):
    #ws.send(f'SERVO:{value},23') # send the command over websocket
    print(f'Servo angle: {value}'+u"\N{DEGREE SIGN}")
    return f'Servo angle: {value}'+u"\N{DEGREE SIGN}"

@app.callback(Output('knob-result2', 'children'), Input('servo-knob2', 'value')) # callback for updating data via knob interaction
def update_knob2(value):
    #ws.send(f'SERVO:{value},23') # send the command over websocket
    print(f'Servo angle: {value}'+u"\N{DEGREE SIGN}")
    return f'Servo angle: {value}'+u"\N{DEGREE SIGN}"

@app.callback(Output('knob-result3', 'children'), Input('servo-knob3', 'value')) # callback for updating data via knob interaction
def update_knob(value):
    #ws.send(f'SERVO:{value},23') # send the command over websocket
    print(f'Servo angle: {value}'+u"\N{DEGREE SIGN}")
    return f'Servo angle: {value}'+u"\N{DEGREE SIGN}"
def toDropOption(name):
    return {"label": name, "value": name}

app.layout = dbc.Container([
    html.H1('Robot Arm Controller'),
    dbc.Row([
        dbc.Col([
           arm_controller,
            # upload for elems
            dcc.Upload(
                #id=f'{APP_ID}_elems_upload',
                multiple=False,
                children=[
                    dbc.Label('Upload Element Mesh'),
                    dbc.Button('Upload Elements', color='primary')
                ]
            ),
            html.H3('Step 2: Upload Your Element Results'),
            # upload for elem data
            dcc.Upload(
                multiple=False,
                children=[
                        dbc.Label('Upload Element Results Data'),
                        dbc.Button('Upload Results', color='primary')
                ]
            ),
            html.H3('Step 3: Play with colors!'),
            dbc.Label('Color Map'),
            dcc.Dropdown(
                options=list(map(toDropOption, presets)),
                value="erdc_rainbow_bright",
            ),
            dbc.Label('Cell Value Threshold'),
            dcc.RangeSlider(
                min=0.,
                max=1.,
                step=.01,
                value=[0., 1.],
                marks={
                    0.: '0%',
                    .25: '25%',
                    .50: '50%',
                    .75: '75%',
                    1.0: '100%'
                },
            ),
            dbc.FormText('% of maximum value in data set'),
            dbc.Checklist(
                options=[
                    {"label": "Show Element Edges", "value": 1}
                ],
                switch=True,
                value=[1]
            )
            ],
            width=4
        ),
        dbc.Col([
            html.Div(
                html.Div(
                    style={"width": "100%", "height": "100%"},
                    children = robotViewer
                ),
                style={"height": "100%"},
            ),
        ]),
    ]),
],
    fluid=True,
    style={"height": "75vh"},
)




# Run the web app
PORT = 8050
def main():
    
    # The reloader has not yet run - open the browser
    if not os.environ.get("WERKZEUG_RUN_MAIN"):
        webbrowser.open_new("http://localhost:{}".format(PORT))

    # Otherwise, continue as normal
    app.run(host="127.0.0.1", port=PORT)

if __name__=='__main__':
    main()