ww = {}
cmd_iot = {}
cmd_elevator = {}
cmd_audio = {}
cmd_wash = {}
cmd_led = {}
cmd_dialogic_1 = {}
cmd_dialogic_2_temperature = {}
cmd_dialogic_2_timer = {}

language_codes = ["zh", "fr", "en", "de"]
output_file_prefix = "../IndexToCommand_"
main_file = "../IndexCommands.h"

FILE_DATA = 0
COMMANDS_LIST = 1
FILE_DATA_TRIMMED = 2
COMMANDS_TRIMMED_LIST = 3

def write_file(header_file, language_code, file_var, file_data, extra=""):
    header_file.writelines(extra + "char " + file_var + language_code + "[] =\n")
    header_file.writelines("{\n    ")
    header_file.writelines(file_data)
    header_file.writelines("\n")
    header_file.writelines("};\n\n")

def generate_if_else_strings(language_code, indentation=""):
    ifelse = indentation + "if (infCMDType == ASR_CMD_IOT)\n" +\
            indentation + "    retString = cmd_iot_"+ language_code +";\n" +\
            indentation + "else if (infCMDType == ASR_CMD_ELEVATOR)\n" +\
            indentation + "    retString = cmd_elevator_"+ language_code +";\n" +\
            indentation + "else if (infCMDType == ASR_CMD_AUDIO)\n" +\
            indentation + "    retString = cmd_audio_"+ language_code +";\n" +\
            indentation + "else if (infCMDType == ASR_CMD_WASH)\n" +\
            indentation + "    retString = cmd_wash_"+ language_code +";\n"

    if language_code == "en":
        ifelse = ifelse +\
            indentation + "else if (infCMDType == ASR_CMD_LED)\n" +\
            indentation + "    retString = cmd_led_"+ language_code +";\n" +\
            indentation + "else if (infCMDType == ASR_CMD_DIALOGIC_1)\n" +\
            indentation + "    retString = cmd_dialogic_1_"+ language_code +";\n"

    return ifelse

def generate_if_else_number(indentation=""):
    return  indentation + "if (infCMDType == ASR_CMD_IOT)\n" +\
            indentation + "    cmd_number = NUMBER_OF_IOT_CMDS;\n" +\
            indentation + "else if (infCMDType == ASR_CMD_ELEVATOR)\n" +\
            indentation + "    cmd_number = NUMBER_OF_ELEVATOR_CMDS;\n" +\
            indentation + "else if (infCMDType == ASR_CMD_AUDIO)\n" +\
            indentation + "    cmd_number = NUMBER_OF_AUDIO_CMDS;\n" +\
            indentation + "else if (infCMDType == ASR_CMD_WASH)\n" +\
            indentation + "    cmd_number = NUMBER_OF_WASH_CMDS;\n" +\
            indentation + "else if (infCMDType == ASR_CMD_LED)\n" +\
            indentation + "    cmd_number = NUMBER_OF_LED_CMDS;\n" +\
            indentation + "else if (infCMDType == ASR_CMD_DIALOGIC_1)\n" +\
            indentation + "    cmd_number = NUMBER_OF_DIALOGIC_CMDS;\n"

def generate_file(dir, language_code, file_var, file_name):

    global output_file_prefix

    with open(dir + language_code + file_name, "rb") as file_ww:
        b_utf16le = file_ww.read()
        b_commands = b_utf16le.decode("UTF-16LE").encode("UTF-8")
        size_of_txt = len(b_commands)
        idx_s = b_commands.find(b'\r\n')
        idx_e = b_commands.find(b'\t<', idx_s)
        cmds = []
        cmds_trimmed = []
        while (idx_s < size_of_txt - 2):
            command = b_commands[idx_s+2:idx_e].decode("UTF-8")

            if "Garb" not in command:
                idx_cmd = command.find(' ^')
                if idx_cmd > 0:
                    cmds.append(command[:idx_cmd])
                else:
                    cmds.append(command)
                    cmds_trimmed.append(command)

            idx_s = b_commands.find(b'\r\n', idx_e)
            idx_e = b_commands.find(b'\t<', idx_s)

        file_data = ',\n    '.join('"{0}"'.format(cmd) for cmd in cmds)
        file_data_trimmed = ',\n    '.join('"{0}"'.format(cmd) for cmd in cmds_trimmed)

        with open(output_file_prefix + language_code + ".h", "a", encoding='utf-8') as header_file:
            write_file(header_file, language_code, file_var, file_data)

        return [file_data, cmds, file_data_trimmed, cmds_trimmed]

def generate_main_file():
    open(main_file, 'w').close()

    with open(main_file, "a", encoding='utf-8') as header_file:
        header_file.writelines(\
                "/*\n" +\
                " * Copyright 2021 NXP.\n" +\
                " * This software is owned or controlled by NXP and may only be used strictly in accordance with the\n" +\
                " * license terms that accompany it. By expressly accepting such terms or by downloading, installing,\n" +\
                " * activating and/or otherwise using the software, you are agreeing that you have read, and that you\n" +\
                " * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the\n" +\
                " * applicable license terms, then you may not retain, install, activate or otherwise use the software.\n" +\
                "*/\n" +\
                "\n" +\
                "#ifndef INDEXCOMMANDS_H_\n" +\
                "#define INDEXCOMMANDS_H_\n" +\
                "\n" +\
                "#include \"sln_local_voice.h\"\n" +\
                "\n" +\
                "/* These defines are currently used only for displaying in the commands shell */\n")

        header_file.writelines("#define NUMBER_OF_WW                " + str(len(language_codes)) + "\n")
        header_file.writelines("#define NUMBER_OF_AUDIO_CMDS        " + str(len(cmd_audio[language_codes[0]][COMMANDS_TRIMMED_LIST])) + "\n")
        header_file.writelines("#define NUMBER_OF_ELEVATOR_CMDS     " + str(len(cmd_elevator[language_codes[0]][COMMANDS_TRIMMED_LIST])) + "\n")
        header_file.writelines("#define NUMBER_OF_IOT_CMDS          " + str(len(cmd_iot[language_codes[0]][COMMANDS_TRIMMED_LIST])) + "\n")
        header_file.writelines("#define NUMBER_OF_WASH_CMDS         " + str(len(cmd_wash[language_codes[0]][COMMANDS_TRIMMED_LIST])) + "\n")

        if "en" in language_codes:
            header_file.writelines("#define NUMBER_OF_LED_CMDS          " + str(len(cmd_led["en"][COMMANDS_TRIMMED_LIST])) + "\n")
            header_file.writelines("#define NUMBER_OF_DIALOGIC_CMDS     " + str(len(cmd_dialogic_1["en"][COMMANDS_TRIMMED_LIST])) + "\n\n")

        for language_code in language_codes:
#            write_file(header_file, language_code, "*ww_", ww[language_code][FILE_DATA], "static ")
            write_file(header_file, language_code, "*cmd_iot_", cmd_iot[language_code][FILE_DATA_TRIMMED], "static ")
            write_file(header_file, language_code, "*cmd_elevator_", cmd_elevator[language_code][FILE_DATA_TRIMMED], "static ")
            write_file(header_file, language_code, "*cmd_audio_", cmd_audio[language_code][FILE_DATA_TRIMMED], "static ")
            write_file(header_file, language_code, "*cmd_wash_", cmd_wash[language_code][FILE_DATA_TRIMMED], "static ")

            if language_code == "en":
                write_file(header_file, language_code, "*cmd_led_", cmd_led[language_code][FILE_DATA_TRIMMED], "static ")
                write_file(header_file, language_code, "*cmd_dialogic_1_", cmd_dialogic_1[language_code][FILE_DATA_TRIMMED], "static ")

## get_cmd_strings function header
        header_file.writelines(\
                "char **get_cmd_strings(asr_language_t asrLang, asr_inference_t infCMDType)\n" +\
                "{\n" +\
                "    char **retString = NULL;\n" +\
                "\n" +\
                "    switch (asrLang)\n" +\
                "    {\n")

# get_cmd_strings function body
        for language_code in language_codes:
            if "en" == language_code:
                header_file.writelines(\
                        "        case ASR_ENGLISH:\n"
                        )
                header_file.writelines(generate_if_else_strings(language_code, "           "))
                header_file.writelines(\
                        "            break;\n"
                        )

            if "zh" == language_code:
                header_file.writelines(\
                        "        case ASR_CHINESE:\n"
                        )
                header_file.writelines(generate_if_else_strings(language_code, "           "))
                header_file.writelines(\
                        "            break;\n"
                        )

            if "de" in language_code:
                header_file.writelines(\
                        "        case ASR_GERMAN:\n"
                        )
                header_file.writelines(generate_if_else_strings(language_code, "           "))
                header_file.writelines(\
                        "            break;\n"
                        )

            if "fr" in language_code:
                header_file.writelines(\
                        "        case ASR_FRENCH:\n"
                        )
                header_file.writelines(generate_if_else_strings(language_code, "           "))
                header_file.writelines(\
                        "            break;\n"
                        )

        header_file.writelines(\
                "        default:\n" +\
                "            retString = cmd_led_en;\n" +\
                "            break;\n" +\
                "    }\n" +\
                "\n" +\
                "    return retString;\n" +\
                "}\n\n")
    # end of get_cmd_strings function

# get_cmd_number function header
        header_file.writelines(\
                "unsigned int get_cmd_number(asr_inference_t infCMDType)\n" +\
                "{\n" +\
                "    unsigned int cmd_number = 0;\n\n")

# get_cmd_number function body
        header_file.writelines(generate_if_else_number("   "))

        header_file.writelines(\
                "\n" +\
                "    return cmd_number;\n" +\
                "}\n\n")
# end of get_cmd_number function

        header_file.writelines(\
                "#endif /* INDEXCOMMANDS_H_ */"\
                )

def generate_files():

    global ww, cmd_iot, cmd_elevator, cmd_audio, cmd_wash, cmd_led, cmd_dialogic_1, cmd_dialogic_2_temperature, cmd_dialogic_2_timer

    for language_code in language_codes:
        open(output_file_prefix + language_code + ".h", 'w').close()

        ww[language_code] = generate_file("../oob_demo_", language_code, "*ww_", "/WW.xml")
        cmd_iot[language_code] = generate_file("../oob_demo_", language_code, "*cmd_iot_", "/CMD_IOT.xml")
        cmd_elevator[language_code] = generate_file("../oob_demo_", language_code, "*cmd_elevator_", "/CMD_ELEVATOR.xml")
        cmd_audio[language_code] = generate_file("../oob_demo_", language_code, "*cmd_audio_", "/CMD_AUDIO.xml")
        cmd_wash[language_code] = generate_file("../oob_demo_", language_code, "*cmd_wash_", "/CMD_WASH.xml")
        if language_code == "en":
            cmd_led[language_code] = generate_file("../oob_demo_", language_code, "*cmd_led_", "/CMD_led.xml")
            cmd_dialogic_1[language_code] = generate_file("../oob_demo_", language_code, "*cmd_dialogic_1_", "/CMD_DIALOGIC_1.xml")
            cmd_dialogic_2_temperature[language_code] = generate_file("../oob_demo_", language_code, "*cmd_dialogic_2_temperature_", "/CMD_DIALOGIC_2_TEMPERATURE.xml")
            cmd_dialogic_2_timer[language_code] = generate_file("../oob_demo_", language_code, "*cmd_dialogic_2_timer_", "/CMD_DIALOGIC_2_TIMER.xml")

        print("\n\n==========LANGUAGE " + language_code + " ==========")
        print("Wake Words: \n\t", ww[language_code])
        print("IoT Commands: \n\t", cmd_iot[language_code])
        print("Elevator Commands: \n\t", cmd_elevator[language_code])
        print("Audio Commands: \n\t", cmd_audio[language_code])
        print("Wash Commands: \n\t", cmd_wash[language_code])
        if language_code == "en":
            print("LED Commands: \n\t", cmd_led[language_code])
            print("Dialogic 1 Commands: \n\t", cmd_dialogic_1[language_code])
            print("Dialogic 2 Temperature Commands: \n\t", cmd_dialogic_2_temperature[language_code])
            print("Dialogic 2 Timer Commands: \n\t", cmd_dialogic_2_timer[language_code])

if __name__ == "__main__":

    generate_files()
    generate_main_file()
