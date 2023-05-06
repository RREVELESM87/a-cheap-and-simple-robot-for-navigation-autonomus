
'''
Este código tiene como objetivo la captura y almacenamiento de imágenes de la cámara en diferentes categorías según la señal 
que reciba del dispositivo conectado a través del puerto serial. También se crea una carpeta con diferentes subcarpetas según 
la categoría en la que se va a almacenar la imagen y se verifica si la carpeta ya existe.
Primero, se importan los módulos necesarios: calendar, threading, traceback, serial, cv2, os, numpy, time, sys y pathlib.
Luego, se establecen las propiedades de la toma de los frames de la cámara y se crea la carpeta donde se almacenarán 
las imágenes en diferentes categorías. Si la carpeta ya existe, se imprime un mensaje de que ya existe.
Se define una función llamada "arduinoHilo" que se ejecutará con hilos. Dentro de esta función, 
se crea un bucle infinito que leerá continuamente la imagen de la cámara. Si no se puede leer la imagen, 
la función se detiene. Si se recibe una señal del dispositivo conectado a través del puerto serial, 
se comprueba qué señal se ha recibido y se almacena la imagen en la categoría correspondiente en la 
carpeta que se ha creado previamente. Se cuentan el número de imágenes almacenadas en cada categoría. 
Luego se vacía el búfer de entrada y se calcula el tiempo transcurrido. 
Por último, si la carpeta ya existe, se imprimen los nombres de las carpetas y se leen las imágenes de las 
subcarpetas de cada categoría. Se almacenan las imágenes en una lista de datos y las etiquetas de categoría en otra lista.
'''


from calendar import c
from threading import Thread
from traceback import FrameSummary
import serial, cv2, os, numpy as np, time
import sys
import pathlib
# Propiedades de la toma de Frames 
camara = cv2.VideoCapture(0)
dataPath_frames  = 'TomaFrames/camaraNativa/Toma2'
print("Guardado en:",dataPath_frames )

# def crear_directorios():
if not os.path.exists(dataPath_frames):

    os.makedirs(dataPath_frames + '/b')
    os.makedirs(dataPath_frames + '/c')
    os.makedirs(dataPath_frames + '/k')

    #Izquierda
    os.makedirs(dataPath_frames + '/a')
    os.makedirs(dataPath_frames + '/d')
    os.makedirs(dataPath_frames + '/f')


    #Centro
    os.makedirs(dataPath_frames + '/w')

########## Creación de función arduinoHilo() para ejecución con Hilos ########

def arduinoHilo():
    print("Ejecucion de arduino con Hilos")
    img_counter_b = 0
    img_counter_c = 0
    img_counter_k = 0
    img_counter_a = 0
    img_counter_d = 0
    img_counter_f = 0
    img_counter_w = 0
    while True:
        inicio = time.time()
        ret,frame = camara.read()
        #frame=cv2.resize(frame,(columnas,filas))
        #frame=cv2.resize(frame,(180,180))
        #cv2.imshow('Imagen',frame)
        
        if not ret:
                print("Fallo camara")
                arduino.close()
                break
        while arduino.inWaiting() == 0:
            pass
        if arduino.inWaiting() > 0:
            answer = (arduino.read())
            #print("---> {}".format(answer.decode("utf-8")))
            if (answer.decode("utf-8")=='a'):
                #Categoria 1
                print("python recibio a")
                img_name = dataPath_frames+'/a'+"/a{}.png".format(img_counter_a)
                cv2.imwrite(img_name, frame)
                #print("{} written!".format(img_name))
                img_counter_a += 1
            ########## Categoría    1-a ###########################  
                                
            ########## Categoría    2-l ###########################
            elif (answer.decode("utf-8")=='b'):
                print("python recibio b")
                img_name = dataPath_frames+'/b'+"/b{}.png".format(img_counter_b)
                cv2.imwrite(img_name, frame)
                #print("{} written!".format(img_name))
                img_counter_b += 1
            ########## Categoría    3-w ########################### 
            elif (answer.decode("utf-8")=='c'):
                print("python recibio c")
                img_name = dataPath_frames+'/c'+"/c{}.png".format(img_counter_c)
                cv2.imwrite(img_name, frame)
                #print("{} written!".format(img_name))
                img_counter_c += 1
            elif (answer.decode("utf-8")=='d'):
                print("python recibio d")
                img_name = dataPath_frames+'/d'+"/d{}.png".format(img_counter_d)
                cv2.imwrite(img_name, frame)
                #print("{} written!".format(img_name))
                img_counter_d += 1
            elif (answer.decode("utf-8")=='f'):
                print("python recibio f")
                img_name = dataPath_frames+'/f'+"/f{}.png".format(img_counter_f)
                cv2.imwrite(img_name, frame)
                #print("{} written!".format(img_name))
                img_counter_f += 1

            elif (answer.decode("utf-8")=='k'):
                print("python recibio k")
                img_name = dataPath_frames+'/k'+"/k{}.png".format(img_counter_k)
                cv2.imwrite(img_name, frame)
                #print("{} written!".format(img_name))
                img_counter_k += 1

            elif (answer.decode("utf-8")=='w'):
                print("python recibio w")
                img_name = dataPath_frames+'/w'+"/w{}.png".format(img_counter_w)
                cv2.imwrite(img_name, frame)
                #print("{} written!".format(img_name))
                img_counter_w += 1



        arduino.flushInput() 
        fin = time.time()
        print(fin-inicio)





# def directorio_creado():  
if os.path.exists(dataPath_frames):  
    print("Ya existe directorio")
    dataPath=dataPath_frames
    #dataPath = "Imgs/Captura/Color"
    dir_list = os.listdir(dataPath)
    print(len(dir_list)," Categorias/Carpetas Encontradas")
    print("Carpetas:", dir_list)
    # labels = []
    # facesData = []
    # label = 0
    labels = []
    facesData = []
    label = 0
    for name_dir in dir_list:
        dir_path = dataPath + "/" + name_dir
        
        for file_name in os.listdir(dir_path):
            image_path = dir_path + "/" + file_name
            #print(image_path)
            image = cv2.imread(image_path, 1)
            #cv2.imshow("Image", image)
            #cv2.waitKey(10)
            facesData.append(image)
            labels.append(label)
        label += 1
        #cv2.destroyAllWindows()
            
    data_dir = pathlib.Path(dataPath)
    image_count = len(list(data_dir.glob('*/*.png')))
    print("Total de Imagenes:",image_count)
    image_count = len(list(data_dir.glob('*/*.png')))
    print(" ********************Total de Imagenes Acumuladas *******************\n",image_count)
    # print("Categorias:"," Cantidad"," Descripcion","        Salida Arduino En Grados")
    print(" 'a': ", np.count_nonzero(np.array(labels) == 0))
    print(" 'w': ", np.count_nonzero(np.array(labels) == 1))
    print(" 'k': ", np.count_nonzero(np.array(labels) == 2))
    print(" 'd': ", np.count_nonzero(np.array(labels) == 3))
    print(" 'c': ", np.count_nonzero(np.array(labels) == 4))
    print(" 'b': ", np.count_nonzero(np.array(labels) == 5))
    print(" 'f': ", np.count_nonzero(np.array(labels) == 6))
    # print(" ********************Total de Imagenes Acumuladas *******************")


    img_counter_a = np.count_nonzero(np.array(labels) == 0)
    img_counter_w = np.count_nonzero(np.array(labels) == 1)
    img_counter_k = np.count_nonzero(np.array(labels) == 2)
    img_counter_d = np.count_nonzero(np.array(labels) == 3)
    img_counter_c = np.count_nonzero(np.array(labels) == 4)
    img_counter_b = np.count_nonzero(np.array(labels) == 5)
    img_counter_f = np.count_nonzero(np.array(labels) == 6)


if __name__ == '__main__':

    print('Ejecutandose. Presione CTRL-C para salir.')
    
    with serial.Serial("/dev/ttyUSB0", 115200, timeout=1) as arduino:

        time.sleep(0.1)  # wait fwor serial to open
        if arduino.isOpen():
            print("{} Arduino UNO Conectado!".format(arduino.port))
            print("{} Conecte Control PS4BT!".format(arduino.port))
            try:
                ard = Thread(target=arduinoHilo, args=())
                ard.start()
                ard.join()
                 # remove data after reading
            except KeyboardInterrupt:
                camara.release()
                arduino.close()
                cv2.destroyAllWindows()
                print("\n Interrupcion por teclado (Detener programa).")