from pyModbusTCP.server import ModbusServer
import ReadTextFile as procText
from time import sleep
import numpy as np
import os
import threading
import socket
from datetime import datetime
import logging

def NewConfiguration(server, route):
        settingRoute = "/home/rpi/Desktop/SetInformation.txt"
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        serverAddress = ("192.168.xxx.xxx", 5000)
        sock.bind(serverAddress)
        sock.listen(1)

        while True:
                print("Waiting for connection...")
                try:
                        connection, clientAddress = sock.accept()
                        print(clientAddress)
                        data = connection.recv(1024).decode()
                        sepData = data.split(';')
                        if (data.startswith("xxxxxx") and len(sepData) == 3):
                                d1, d2, d3 = sepData
                                chanInformation = [d2, d3]
                                procText.writeText(settingRoute, chanInformation)
                        else:
                                print("DATO INVALIDO...")

                finally:
                        connection.close()

# **********************************************************************
server = ModbusServer("192.168.xxx.xxx", 502, no_block = True)

try:
        horaInicio = datetime.now().strftime("Fecha y hora de Inicio: %Y-%m-%d %H:%M:%S")
        print("HORA INICIO: ", horaInicio)
        logging.basicConfig(filename='/home/rpi/Desktop/LoggModbusServer.log', encoding='utf-8', level=logging.DEBUG)
        logging.info(horaInicio)
        server.start()
        print("Server Online")
        inputRoute = "/home/rpi/Desktop/SPOTCHANNELS.txt"
        holdingRoute = "/home/rpi/Desktop/PARAMCHANNELS.txt"
        flag = False

        while True:
                if os.stat(inputRoute) != 0:
                        dataInput = procText.readText(inputRoute)
                        dataInput = dataInput[0:18]
                        #print(dataInput)
                        dataInput = [int(float(element)*100) for element in dataInput]
                        dataInput, indexInputPosition = procText.GreatDataCod(dataInput)
                        server.data_bank.set_input_registers(0, dataInput)

                if os.stat(holdingRoute) != 0:
                        dataHolding = procText.readText(holdingRoute)
                        dataHolding = dataHolding[0:29]
                        #print(dataHolding)
                        dataHolding = [int(float(element)*100) for element in dataHolding]
                        dataHolding, indexHoldingPosition = procText.GreatDataCod(dataHolding)
                        server.data_bank.set_input_registers(18, dataHolding)

                if flag == False:
                        thread1 = threading.Thread(target = NewConfiguration, args = (server, holdingRoute))
                        thread1.start()
                        flag = True
                sleep(0.5)
except Exception as e:
        print("SERVER ERROR: ", e)
        horaFalla = datetime.now().strftime("Fecha y hora de erro: %Y-%m-%d %H:%M:%S")
        logging.error(horaFalla)
        print("Shutdown Server...")
        server.stop()
        thread1.join(timeout = 1.0)
        print("Server offline")
        exit()