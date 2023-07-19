#! /usr/bin/env python3
'''
Copyright 2019, 2021-2022 NXP.
This software is owned or controlled by NXP and may only be used strictly in accordance with the
license terms that accompany it. By expressly accepting such terms or by downloading, installing,
activating and/or otherwise using the software, you are agreeing that you have read, and that you
agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
applicable license terms, then you may not retain, install, activate or otherwise use the software.
'''

import argparse
import serial
import os
import sys
import shutil
import wave
from scipy.io import wavfile
import threading
import signal

# Configuration variables. Adjust to your needs
CONVERT_RAW_TO_WAV = True
KEEP_RAW_FILES = False

# Global variables
STOP_PROCESS = False
SER = None
MICS_CNT = '3'
FRAME_SAMPLE_CNT = 160
FRAME_HEADER_LEN = 8
MAX_LENGTH_CONVERT_BYTE = 3 * 1024 * 1024 * 1024 #3GB

def signal_handler(sig, frame):
    global STOP_PROCESS

    print('--> CTRL + C pressed')
    STOP_PROCESS = True

def wait_cancel_from_user():
    global STOP_PROCESS

    print("Type 'q' and press Enter to cancel the process >>\r\n")

    try:
        for line in sys.stdin:
            cmd = line.rstrip()
            if cmd == 'q':
                print("--> User pressed 'q'")
                break
    except:
        print("--> [WARNING] wait_cancel_from_user exception")

    STOP_PROCESS = True
    SER.cancel_read()
    print("--> wait_cancel_from_user task done")

def capture_g_afe(mic_sample_size):
    global STOP_PROCESS

    while(1):
        if STOP_PROCESS:
            print("--> Recording Stopped")
            try:
                SER.write(b'c')
            except:
                print("--> Failed to send 'cancel' to the board")
            break

        try:
            if STOP_PROCESS == False:
                ser_data = SER.read(FRAME_HEADER_LEN)
                runtime_info_file.write(ser_data)
            if STOP_PROCESS == False:
                ser_data = SER.read(FRAME_SAMPLE_CNT * mic_sample_size)
                mic1_file.write(ser_data)

            if STOP_PROCESS == False:
                ser_data = SER.read(FRAME_SAMPLE_CNT * mic_sample_size)
                mic2_file.write(ser_data)

            if STOP_PROCESS == False:
                if MICS_CNT == '3':
                    ser_data = SER.read(FRAME_SAMPLE_CNT * mic_sample_size)
                    mic3_file.write(ser_data)

            if STOP_PROCESS == False:
                ser_data = SER.read(FRAME_SAMPLE_CNT * 2)
                amp_file.write(ser_data)

            if STOP_PROCESS == False:
                ser_data = SER.read(FRAME_SAMPLE_CNT * 2)
                clean_audio_file.write(ser_data)


            if STOP_PROCESS == False:
                if not ser_data:
                    print("--> [WARNING] ser_data empty")
                    continue

        except Exception as e:
            print("--> Recording Error")
            STOP_PROCESS = True

def convert_raw_to_wav(file_path, sample_size=2):
    with open(file_path, "rb") as inp_f:
        data_size = os.path.getsize(file_path)
        datas = inp_f.read()

        if data_size <= MAX_LENGTH_CONVERT_BYTE:
            new_file_path = file_path[:-3] + "wav"
            with wave.open(new_file_path, "wb") as out_f:
                out_f.setnchannels(1)
                out_f.setsampwidth(sample_size) # number of bytes
                out_f.setframerate(16000)
                out_f.writeframesraw(datas)
        else:
            data_convert_length = 0
            dump_wav_cnt = 1
            while(data_convert_length < data_size):
                new_file_path = file_path[:-4] + "_" + str(dump_wav_cnt) + ".wav"
                with wave.open(new_file_path, "wb") as out_f:
                    if data_convert_length + MAX_LENGTH_CONVERT_BYTE >= data_size:
                        data = datas[data_convert_length:]
                    else:
                        data = datas[data_convert_length:data_convert_length+MAX_LENGTH_CONVERT_BYTE]
                    out_f.setnchannels(1)
                    out_f.setsampwidth(sample_size) # number of bytes
                    out_f.setframerate(16000)
                    out_f.writeframesraw(data)
                    dump_wav_cnt += 1
                    data_convert_length += MAX_LENGTH_CONVERT_BYTE

    if not KEEP_RAW_FILES:
        os.remove(file_path)


""" Parse the provided parameters """
parser = argparse.ArgumentParser()
parser.add_argument('-m', '--microphones', type=int, default=2, choices=[2, 3], help="Set the number of microphones.")
parser.add_argument('-mss', '--mic-sample-size', type=int, default=4, choices=[2, 4], help="Set microphone one sample's size in bytes.")
parser.add_argument('-p', '--port', type=str, required=True, help="Set the port the device is connected. Example for Window COM3, for Linux /dev/ttyACM3.")
parser.add_argument('-f', '--folder', type=str, required=True, help="Folder where to save audio files. Note: overwrite existing folder.")
parser.add_argument('-t', '--type', type=str, required=False, help="Just keep this parameter for back compatible. it is not used any more.")
args = parser.parse_args()

# Get the Number of microphones
MICS_CNT = str(args.microphones)

# Get the Test name
test_name = args.folder

# Delete the Test folder with the same name (if exists)
try:
    shutil.rmtree(test_name)
except:
    pass

try:
    os.mkdir(test_name)
except OSError:
    print("--> ERROR: Creation of the directory %s failed" % test_name)
    sys.exit(1)

MIC1_STREAM_PATH                = test_name + "/" + test_name + "_mic1.raw"
MIC2_STREAM_PATH                = test_name + "/" + test_name + "_mic2.raw"
MIC3_STREAM_PATH                = test_name + "/" + test_name + "_mic3.raw"
AMP_STREAM_PATH                 = test_name + "/" + test_name + "_amp.raw"
CLEAN_STREAM_PATH               = test_name + "/" + test_name + "_clean_processed_audio.raw"
AUDIO_RUNTIME_INFO              = test_name + "/" + test_name + "_runtime_info.bin"


mic1_file        = open(MIC1_STREAM_PATH,  "wb")
mic2_file        = open(MIC2_STREAM_PATH,  "wb")
if MICS_CNT == '3':
    mic3_file = open(MIC3_STREAM_PATH, "wb")
amp_file         = open(AMP_STREAM_PATH,   "w+b")
clean_audio_file = open(CLEAN_STREAM_PATH, "wb")
runtime_info_file = open(AUDIO_RUNTIME_INFO, "wb")


# Get the Serial Port to the device
port_num = args.port

# Configure the serial connections (the parameters differs on the device you are connecting to)
try:
    SER = serial.Serial(
        port=port_num,
        baudrate=2048000,
        parity=serial.PARITY_NONE,
        stopbits=serial.STOPBITS_ONE,
        bytesize=serial.EIGHTBITS,
        write_timeout=1,
    )
except:
    print("--> ERROR: Failed to connect to " + port_num + " serial port")
    sys.exit(1)

signal.signal(signal.SIGINT, signal_handler)

try:
    SER.write(b's')
except:
    print("--> WARNING: Start write timeout. Probably audio_dump was already started...")


# Start recording. User has to type "stop" to cancel the process
stop_thread = threading.Thread(target=wait_cancel_from_user, daemon=True)
stop_thread.start()
capture_g_afe(args.mic_sample_size)
stop_thread.join(timeout=0.1)

# Close the files
mic1_file.close()
mic2_file.close()
if MICS_CNT == '3':
    mic3_file.close()
amp_file.close()
clean_audio_file.close()
runtime_info_file.close()


# Convert RAW files to WAV files
if CONVERT_RAW_TO_WAV:
    print("--> Converting RAW to WAV")

convert_raw_to_wav(MIC1_STREAM_PATH, args.mic_sample_size)
convert_raw_to_wav(MIC2_STREAM_PATH, args.mic_sample_size)
if MICS_CNT == '3':
    convert_raw_to_wav(MIC3_STREAM_PATH, args.mic_sample_size)
convert_raw_to_wav(AMP_STREAM_PATH)
convert_raw_to_wav(CLEAN_STREAM_PATH)