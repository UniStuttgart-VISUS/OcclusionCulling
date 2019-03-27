import matplotlib.pyplot as plt
import sys
import os
import math

if __name__ == '__main__':
    
    path = sys.argv[1]
    value_to_compare = sys.argv[2]
    output = sys.argv[3]
    evaluation_names = []
    legend_names = []
    result = []
    result_1 = []
    result_2 = []
    
    
    plt.rcParams.update({'pgf.rcfonts': False, 'font.size': 9, 'font.family':'serif'})
    width=4
    height=width/1.618
    plt.figure(figsize=(width, height))
    '''
    bottom=0.22
    left=0.18
    plt.gcf().subplots_adjust(bottom=bottom)
    plt.gcf().subplots_adjust(left=left)
    '''
    
    # Read names of the evaluations
    for i in range(4, 4+int((len(sys.argv)-4)/2)):
        evaluation_names.append(sys.argv[i])
    
    # Read legend names
    for i in range(4 + int((len(sys.argv)-4)/2), len(sys.argv)):
        legend_names.append(sys.argv[i])
    
    # Read input data
    for i in range(len(evaluation_names)):
        if value_to_compare == 'Percentage culled':
            file_1 = path + evaluation_names[i] + '_plots/' + evaluation_names[i] + '_' + 'Culled Tries' + '_Data'   
            file_2 = path + evaluation_names[i] + '_plots/' + evaluation_names[i] + '_' + 'Visible Tries' + '_Data'
            
            result.append([])
            result_1.append([])
            result_2.append([])
            
            with open(file_1) as f:
                # Skip names and first frame
                next(f)
                next(f)
                next(f)
                for line in f.read().split('\n'):
                    if line == '':
                        break
                    result_1[i].append(float(line))
                    
            with open(file_2) as f:
                # Skip names and first frame
                next(f)
                next(f)
                next(f)
                for line in f.read().split('\n'):
                    if line == '':
                        break
                    result_2[i].append(float(line))
            continue
        
        
        file = path + evaluation_names[i] + '_plots/' + evaluation_names[i] + '_' + value_to_compare + '_Data'
        result.append([])
        with open(file) as f:
            # Skip names and first frame
            next(f)
            next(f)
            next(f)
            for line in f.read().split('\n'):
                if line == '':
                    break
                result[i].append(float(line))
    
    # Create directory
    if not os.path.isdir(output):
        os.mkdir(output)
    
    # Create x-Axes data
    if value_to_compare == 'Percentage culled':
        frames = list(range(0, len(result_1[0])))
    else:
        frames = list(range(0, len(result[0])))

    # Create and save the plot
    if value_to_compare == 'Percentage culled':
        for i in range(len(result)):
            for j in range(len(result_1[i])):
                culled = result_1[i][j]
                visible = result_2[i][j]
                percentage_culled = round(culled/(visible+culled), 4)
                result[i].append(percentage_culled)
            plt.plot(frames, result[i], label=legend_names[i])
        plt.ylim(0,1)
        plt.ylabel('% Tries culled', fontsize=9)
        plt.xlabel('Frame Number', fontsize=9)
    else:
        for i in range(len(result)):
            plt.plot(frames, result[i], label=legend_names[i])
        plt.ylabel(value_to_compare, fontsize=9)
        plt.xlabel('Frame Number', fontsize=9)
    
    '''
    if len(evaluation_names) > 3:
        plt.legend(loc='lower center', bbox_to_anchor=(0.5, -0.53), ncol=3)
    else:
        plt.legend(loc='lower center', bbox_to_anchor=(0.5, -0.43), ncol=3)
    '''
    
    plt.xticks([])
    plt.yticks([])
	  
    #plt.legend(loc='upper right', bbox_to_anchor=(1.01, 1.01), prop={'size': 6})
    plt.tight_layout()
    plt.savefig(output + '/' + output + '_' + value_to_compare + '.pdf')
    #plt.savefig(output + '/' + output + '_' + value_to_compare + '.pgf')