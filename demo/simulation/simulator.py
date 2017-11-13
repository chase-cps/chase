import sys;
from automaton import TulipStrategy;
import matplotlib.pyplot as plt
import numpy as np

input_values = {};
RG1_state_log = [];
LG1_state_log = [];
contactor_1_log = [];
contactor_2_log = [];
contactor_3_log = [];


def manage_events(time):
    if time == 0.03:
        input_values['RG1_state'] = 0;


def transition(controller):
    output_values = controller.move(**input_values);

    if(output_values['contactor_1_intention'] == input_values['contactor_1_state']):
        input_values['contactor_1_switch_off_counter'] = 0;
        input_values['contactor_1_switch_on_counter'] = 0;
    else:
        if output_values['contactor_1_intention'] == 1 :
            input_values['contactor_1_switch_on_counter'] += 1;
        else:
            input_values['contactor_1_switch_off_counter'] += 1;

    if(output_values['contactor_2_intention'] == input_values['contactor_2_state']):
        input_values['contactor_2_switch_off_counter'] = 0;
        input_values['contactor_2_switch_on_counter'] = 0;       
    else:
        if output_values['contactor_2_intention'] == 1 :
            input_values['contactor_2_switch_on_counter'] += 1;
        else:
            input_values['contactor_2_switch_off_counter'] += 1;

    if(output_values['contactor_3_intention'] == input_values['contactor_3_state']):
        input_values['contactor_3_switch_off_counter'] = 0;
        input_values['contactor_3_switch_on_counter'] = 0;
    else:
        if output_values['contactor_3_intention'] == 1 :
            input_values['contactor_3_switch_on_counter'] += 1;
        else:
            input_values['contactor_3_switch_off_counter'] += 1;

    if( input_values['contactor_1_switch_off_counter'] == 1 ):
        input_values['contactor_1_state'] = 0;
        input_values['contactor_1_switch_off_counter'] = 0;
    if( input_values['contactor_2_switch_off_counter'] == 1 ):
        input_values['contactor_2_state'] = 0;
        input_values['contactor_2_switch_off_counter'] = 0;
    if( input_values['contactor_3_switch_off_counter'] == 1 ):
        input_values['contactor_3_state'] = 0;     
        input_values['contactor_3_switch_off_counter'] = 0;

    if( input_values['contactor_1_switch_on_counter'] == 1 ):
        input_values['contactor_1_state'] = 1;
        input_values['contactor_1_switch_on_counter'] = 0;
    if( input_values['contactor_2_switch_on_counter'] == 1 ):
        input_values['contactor_2_state'] = 1;
        input_values['contactor_2_switch_on_counter'] = 0;
    if( input_values['contactor_3_switch_on_counter'] == 1 ):
        input_values['contactor_3_state'] = 1;     
        input_values['contactor_3_switch_on_counter'] = 0;
    return output_values;


def print_system(output_values):
    print("ENVIRONMENT:");
    print("RG1:\t" 
            + str(input_values['RG1_state']));
    print("LG1:\t" 
            + str(input_values['LG1_state']));

    print("contactor 1\t" 
            + str(input_values['contactor_1_state']));
    print("contactor 2\t" 
            + str(input_values['contactor_2_state']));
    print("contactor 3\t" 
            + str(input_values['contactor_3_state']));

def trace_system(t):
    RG1_state_log.append(int(str(input_values['RG1_state'])));
    LG1_state_log.append(int(str(input_values['LG1_state'])));
    contactor_1_log.append(int(str(input_values['contactor_1_state'])));
    contactor_2_log.append(int(str(input_values['contactor_2_state'])));
    contactor_3_log.append(int(str(input_values['contactor_3_state'])));
    

def plot_execution():
    plt.ylabel('variable value')
    plt.yticks(np.arange(0, 1.1, 1));
    plt.xticks(np.arange(0,0.2,0.01));
    plt.show() 
 

def initialize_input_values():
    input_values['contactor_1_state'] = 1;
    input_values['contactor_2_switch_on_counter'] =  0;
    input_values['contactor_1_switch_off_counter'] = 0;
    input_values['RG1_state'] = 1;
    input_values['contactor_2_state'] = 1;
    input_values['contactor_3_switch_off_counter'] = 0; 
    input_values['contactor_3_switch_on_counter'] = 0;
    input_values['contactor_3_state'] = 0;
    input_values['LG1_state'] = 1;
    input_values['contactor_2_switch_off_counter'] = 0;
    input_values['contactor_1_switch_on_counter'] = 0;

def main(args):
    # Initialization phase.
    step = 0.01;
    time = 0.0;
    stop_time = 0.2;

    controller = TulipStrategy();

    initialize_input_values();
    print(input_values);
    while (time < stop_time):
        output_values = transition(controller);
        print("Status after " + str(time) + " seconds:");
        print_system(output_values);
        trace_system(time);
        manage_events(time);

        time = time + step;

    print
    print(str("RG_state =\t") + str(RG1_state_log));
    print(str("LG_state =\t") + str(LG1_state_log));
    print(str("contactor_1 =\t") + str(contactor_1_log));
    print(str("contactor_2 =\t") + str(contactor_2_log));
    print(str("contactor_3 =\t") + str(contactor_3_log));

    plot_execution();
    

if __name__ == "__main__":
    main(sys.argv)

    
