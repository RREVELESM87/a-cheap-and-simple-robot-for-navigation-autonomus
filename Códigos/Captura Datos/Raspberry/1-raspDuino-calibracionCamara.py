import cv2
import argparse
import numpy as np
import serial
import time


#Capturar frames
def capturar_frames(camera,columnas,filas):

    captura = cv2.VideoCapture(camera)

    
    
    frame_width = captura.get(cv2.CAP_PROP_FRAME_WIDTH)
    frame_height = captura.get(cv2.CAP_PROP_FRAME_HEIGHT)
    fps = captura.get(cv2.CAP_PROP_FPS)

    # Print these values:
    print("Resolucion Nativa de la camara")
    print("ANCHO COLUMNAS: '{}'".format(frame_width))
    print("ALTO FILAS: '{}'".format(frame_height))
    print("FPS : '{}'".format(fps))

    # Check if camera opened successfully
    if captura.isOpened() is False:
        print("Error opening the camera")
    # Index to save current frame
    frame_index = 0 


    while (captura.isOpened()):
        ret, imagen = captura.read()
        if ret == True:
            #cv2.imshow('Color', imagen)
            #imagen_gris = cv2.cvtColor(imagen, cv2.COLOR_BGR2GRAY)
            #cv2.imshow('Gris', imagen_gris)
            #columnas,filas
            imagen_scaled = cv2.resize(imagen, (columnas, filas))
            #cv2.imshow('Scaled', imagen_scaled)
            #imagen_scaled_flip = cv2.flip(imagen_scaled, 1)
            #cv2.imshow('Scaled & Flip Horizontal', imagen_scaled_flip)

            for i in range (0,3,1):
                #frame_name = "Frames/Raspberry/Color/Color{}.png".format(frame_index)
                #gray_frame_name = "Frames/Raspberry/Gris/Gris{}.png".format(frame_index)
                scaled_frame_name = "Frames/Raspberry/Scaled/Scaled{}.png".format(frame_index)
                #scaled_frame_name_flip = "Frames/Raspberry/Flip/Flip{}.png".format(frame_index)
                #cv2.imwrite(frame_name, imagen)
                #cv2.imwrite(gray_frame_name, imagen_gris)
                cv2.imwrite(scaled_frame_name,imagen_scaled)
                #cv2.imwrite(scaled_frame_name_flip,imagen_scaled_flip)
                frame_index += 1


            if cv2.waitKey(1) & 0xFF == ord('s'):
                break
        else: break
    captura.release()
    cv2.destroyAllWindows()
    return captura



img_counter_ang255 =0
img_counter_ang0 = 0








if __name__ == '__main__':
    capturar_frames(0,640,180)

