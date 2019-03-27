import os

if __name__ == '__main__':
    
    methods = [
            'OGL_0',
            'OGL_1',
            'OGL_2',
            'OGL_3',
            'OGL_4'
            ]
    
    values = [
            'FPS',
            'DepthTest Time',
            'Rasterize Time'
            ]
    
    table_description_1 = [
            '320x192',
            '640x360',
            '1280x720',
            '1920x1080',
            '2432x1440'
            ]
    
    table_description_2 = [
            'FPS',
            'DepthTest time (ms)',
            'Rasterize time (ms)'
            ]
    
    evaluation = 'Evaluation_4'
    table = 'Tabelle_4'
    
    lines = []
    for value in values:
        lines.append('')
    
    if not os.path.isdir('Tabellen'):
        os.mkdir('Tabellen')
    
    output = open('Tabellen/' + table, 'w')
    output_data = open('Tabellen/' + table + '_Data', 'w')
    
    output.write('\hline\n')
    output.write('{:<20} '.format(''))
    for description in table_description_1:
        output.write('& {:<10}'.format(description))
    output.write('\\\\ \hline\n')
    
    for i in range(len(table_description_2)):
        lines[i]+='{:<20} '.format(table_description_2[i])
    
    print('{:>36} {:>14} {:>14}'.format('Avg', 'Min', 'Max'))
    output_data.write('{:>36} {:>14} {:>14}\n'.format('Avg', 'Min', 'Max'))
    
    for method in methods:
        for i in range(len(values)):
            if values[i] != 'Percentage culled':
                f = evaluation + '/' + method + '_plots/' + method + '_' + values[i] + '_Data'
                sum = 0
                counter = 0
                max = 0
                min = 1000000000
                with open(f) as f:
                    next(f)
                    next(f)
                    for line in f.read().split('\n'):
                        if line == '':
                            break
                        sum+=float(line)
                        if float(line) > max:
                            max = float(line)
                        if float(line) < min:
                            min = float(line)
                        counter+=1
                    
                    if values[i] == 'Culling time' or values[i] == 'DepthTest Time' or values[i] == 'Rasterize Time':
                        print('{:<10} {:<16} {:>8} {:>14} {:>14}'.format(method, values[i], round(1000*(sum/counter),2), min, max))
                        output_data.write('{:<10} {:<16} {:>8} {:>14} {:>14}\n'.format(method, values[i], round(1000*(sum/counter),2), min, max))
                        lines[i]+='& {:<10}'.format(round(1000*(sum/counter),2))
                    elif values[i] == 'Draw Calls':
                        print('{:<10} {:<16} {:>8} {:>814} {:>14}'.format(method, values[i], round(sum/counter), min, max))
                        output_data.write('{:<10} {:<16} {:>8} {:>14} {:>14}\n'.format(method, values[i], round(sum/counter), min, max))
                        lines[i]+='& {:<10}'.format(round(sum/counter))
                    else:
                        print('{:<10} {:<16} {:>8} {:>14} {:>14}'.format(method, values[i], round(sum/counter,2), min, max))
                        output_data.write('{:<10} {:<16} {:>8} {:>14} {:>14}\n'.format(method, values[i], round(sum/counter,2), min, max))
                        lines[i]+='& {:<10}'.format(round(sum/counter,2))
            else:
                f_culled = evaluation + '/' + method + '_plots/' + method + '_Culled Tries_Data'
                f_visible = evaluation + '/' + method + '_plots/' + method + '_Visible Tries_Data'
                culled = []
                visible = []
                with open(f_culled) as f_culled:
                    next(f_culled)
                    next(f_culled)
                    for line in f_culled.read().split('\n'):
                        if line == '':
                            break
                        culled.append(float(line))
                with open(f_visible) as f_visible:
                    next(f_visible)
                    next(f_visible)
                    for line in f_visible.read().split('\n'):
                        if line == '':
                            break
                        visible.append(float(line))
                
                sum = 0
                counter = 0
                max = 0
                min = 1000000000
                for j in range(len(culled)):
                    percentage = culled[j]/(culled[j]+visible[j])
                    sum+=float(percentage)
                    if float(percentage) > max:
                        max = round(float(percentage)*100, 2)
                    if float(percentage) < min:
                        min = round(float(percentage)*100, 2)
                    counter+=1
                print('{:<10} {:<16} {:>8} {:>14} {:>14}'.format(method, 5, round(100*(sum/counter),2), min, max))
                output_data.write('{:<10} {:<16} {:>8} {:>14} {:>14}\n'.format(method, '% Culled', round(100*(sum/counter),2), min, max))
                lines[i]+='& {:<10}'.format(round(100*(sum/counter),2))
        print()
        output_data.write('\n')
    
    for i in range(len(lines)):
         lines[i]+='\\\\'
    for i in range(len(lines)-1):
        lines[i]+=' \hline'
    for i in range(len(lines)):  
        output.write(lines[i] + '\n')
    output.write('\hline')