import matplotlib.pyplot as plt
import sys
import os

if __name__ == '__main__':
    
    input_file = sys.argv[1]
    path = sys.argv[2]
    frames = int(sys.argv[3])
    result = []
    
    # Read input data
    with open(input_file) as f:
        for line in f.read().split('\n'):
            values = line.split('<')
            if values[0] == 'Frame-ID':
                for i in range(len(values)):
                    result.append([values[i]])
                continue
            elif values[0] == 'F-ID':
                for i in range(len(values)):
                    result[i].append(values[i])
                continue
            
            values = line.split(' ')
            if values[0] == "":
                break
            
            for i in range(len(values)):
                result[i].append(float(values[i]))
    
    # Create directory
    if not os.path.isdir(path + input_file + '_plots'):
        os.mkdir(path + input_file + '_plots')
    
    # Create interactive Graph
    for i in range(1, len(result)-1):
        plt.subplot(len(result), 1, i)
        name = result[i][1]
        plt.ylabel(name)
        plt.plot(result[i][-frames:])
    
    plt.xlabel('Frame Number') 
        
    plt.show(block=True)
    
    # Create png for every graph
    for i in range(1, len(result)):
        fig, plot = plt.subplots()
        name = result[i][0]
        plt.ylabel(name)
        plt.xlabel('Frame Number')   
        plot.plot(result[i][-frames:])
        plt.savefig(input_file + '_plots/' + input_file + '_' + name + '.png', bbox_inches = "tight")
        
        # Save graph data
        f = open(input_file + '_plots/' + input_file + '_' + name + '_Data',"w+")
        for j in range(len(result[i])):
            f.write(str(result[i][j]) + '\n')