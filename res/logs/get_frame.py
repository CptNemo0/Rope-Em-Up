import numpy as np;
import os
import matplotlib.pyplot as plt
import keyboard
import time

def on_key_event(event):
    if event.event_type == keyboard.on_release_key:
        print(f"Key {event.name} pressed")

def callback():
    time.sleep(0.02)
    plt.close()
    plt.cla()

    script_dir = os.path.dirname(os.path.abspath(__file__))
    
    coll_a_debug = np.genfromtxt(f"{script_dir}\\coll_a_debug.csv", delimiter=",")
    coll_b_debug = np.genfromtxt(f"{script_dir}\\coll_b_debug.csv", delimiter=",")
    polygon_debug = np.genfromtxt(f"{script_dir}\\polygon_debug.csv", delimiter=",")

    
    plt.scatter(coll_a_debug[:, 0], coll_a_debug[:, 1], color = "red")
    plt.scatter(coll_b_debug[:, 0], coll_b_debug[:, 1], color = "green")
    plt.scatter(polygon_debug[:, 0], polygon_debug[:, 1], color = "blue")

    plt.scatter(0.0, 0.0, color = "black")

    plt.show()

    

def main():
    #keyboard.add_hotkey('p', callback)
    #keyboard.wait('esc')

    while True:
        script_dir = os.path.dirname(os.path.abspath(__file__))
        
        coll_a_debug = np.genfromtxt(f"{script_dir}\\coll_a_debug.csv", delimiter=",")
        coll_b_debug = np.genfromtxt(f"{script_dir}\\coll_b_debug.csv", delimiter=",")
        polygon_debug = np.genfromtxt(f"{script_dir}\\polygon_debug.csv", delimiter=",")

        
        plt.scatter(coll_a_debug[:, 0], coll_a_debug[:, 1], color = "red")
        plt.scatter(coll_b_debug[:, 0], coll_b_debug[:, 1], color = "green")
        plt.scatter(polygon_debug[:, 0], polygon_debug[:, 1], color = "blue")

        plt.scatter(0.0, 0.0, color = "black")

        plt.gca().set_aspect('equal', adjustable='box')
        plt.show()
        plt.cla()
    

if __name__ == "__main__":
    main()