
from os import listdir
from os.path import isfile, join
from keras.preprocessing import image
from keras.preprocessing.image import ImageDataGenerator
import numpy as np
import inception_v3 as inception
def runOnFile(file_path,model):
    img_size = (299, 299)
    img = image.load_img(file_path, target_size=img_size)
    x = image.img_to_array(img)
    x = np.expand_dims(x, axis=0)

    x = inception.preprocess_input(x)
    preds = model.predict(x)
    return preds

def runOnDirectory(directory, model, maxItems = 0):
    img_files = [f for f in listdir(directory) if isfile(join(directory, f))]
    result = []
    count=0
    for f in img_files:
        file_path = directory+"/"+f
        p = runOnFile(file_path,model)
        result.append([f,file_path,p[0]])
        count=count+1
        if(count==maxItems):
            break
    print("Successfuly processed "+ str(count) + " items in directory: "+ directory)
    return result
def getMaxIndex(predicts):
    max = predicts[0]
    maxIndex = 0
    for i in range(predicts.size):
        if predicts[i] > max:
            max = predicts[i]
            maxIndex = i
    return maxIndex

def attachLabel(imagePredicts, predictsIndex, labelList):
    for img in imagePredicts:
        maxIndex = getMaxIndex(img[predictsIndex])
        img.append(labelList[maxIndex])
    return

# In[76]:

def generateHtml(imagePredicts, htmlFilename, imgPerRow=5):
    entry_template = '''<td align="center">
            <table>
                <tr>
                    <img src="{imgPath}", height="100", width="100"/>
                </tr>
                <tr>
                    <th>{FileName}</th>
                </tr>
                <tr>
                    <th>{Predicts}</th>
                </tr>
                <tr>
                    <th>{Label}</th>
                </tr>
            </table>
        </td>'''
    page_templage='''
    <!DOCTYPE html>
    <html>
    <head>
    <style>
    table, th, td {
        border: 1px solid black;
    }
    </style>
    </head>
    <body>
    {MainTable}
    </body>
    </html>

    '''
    count=0
    rows_html=""
    row_html=""

    for e in imagePredicts:
        count+=1
        entry = entry_template.format(imgPath = e[1],FileName=e[0],Predicts=e[2],Label=e[3])
        row_html+=entry
        #print(entry)
        if(count%imgPerRow==0):
            rows_html += "<tr>"+row_html+"</tr>"
            row_html=""
    table_html = "<table>"+rows_html+"</table>"
    page_html=page_templage.replace("{MainTable}",table_html)
    with open(htmlFilename, "w") as text_file:
        text_file.write(page_html)
    print("Output at "+htmlFilename)
    return page_html

