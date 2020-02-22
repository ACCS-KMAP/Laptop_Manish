import serial
import time
import pickle
import warnings
warnings.filterwarnings("ignore")

def columnavg(values):
    result = [0]*6
    for j in range(6):
        for i in range(len(values)):
            result[j] += values[i][j]
        result[j] = result[j]/14
    return result
  

values = []
s = serial.Serial("COM3",9600)
s.baudrate=9600
init_time = time.time()
counter = 0
filename = 'accsfall2.sav'

  
# Use the loaded model to make predictions

fall_model = pickle.load(open(filename, 'rb'))

while((time.time() - init_time)< 2500):
    
     #Establish Serial Communication
    t = s.readline() #Readline
    val = str(t).strip("b'").strip("\\n").strip("\\r") #Strip Unnecessary Values
    splitted = val.split(',')
    splitted = [float(x) for x in splitted]
    #print(splitted)
    values.append(splitted)
    if(len(values)==30):
        del(values[0])
        datapoint = columnavg(values)

        if(counter%5 == 0):
            value_predicted = fall_model.predict([datapoint])
            if(value_predicted == [1]):
                print("Fall Detected")
            #print(datapoint)
    counter += 1




