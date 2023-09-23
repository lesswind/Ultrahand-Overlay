/********************************************************************************
 * File: utils.hpp
 * Author: ppkantorski
 * Description:
 *   This header file contains utility functions and macros used in the
 *   Ultrahand Overlay project. These functions and macros include definitions for
 *   various button keys, path variables, and command interpretation and execution.
 *
 *   For the latest updates and contributions, visit the project's GitHub repository.
 *   (GitHub Repository: https://github.com/ppkantorski/Ultrahand-Overlay)
 *
 *  Copyright (c) 2023 ppkantorski
 *  All rights reserved.
 ********************************************************************************/

#pragma once
#include <switch.h>
#include <sys/stat.h>
#include <dirent.h>
#include <fnmatch.h>
#include <get_funcs.hpp>
#include <path_funcs.hpp>
#include <ini_funcs.hpp>
#include <hex_funcs.hpp>
#include <download_funcs.hpp>
#include <json_funcs.hpp>

/**
 * @brief Shutdown modes for the Ultrahand-Overlay project.
 *
 * These macros define the shutdown modes used in the Ultrahand-Overlay project:
 * - `SpsmShutdownMode_Normal`: Normal shutdown mode.
 * - `SpsmShutdownMode_Reboot`: Reboot mode.
 */
#define SpsmShutdownMode_Normal 0
#define SpsmShutdownMode_Reboot 1

/**
 * @brief Key mapping macros for button keys.
 *
 * These macros define button keys for the Ultrahand-Overlay project to simplify key mappings.
 * For example, `KEY_A` represents the `HidNpadButton_A` key.
 */
#define KEY_A HidNpadButton_A
#define KEY_B HidNpadButton_B
#define KEY_X HidNpadButton_X
#define KEY_Y HidNpadButton_Y
#define KEY_L HidNpadButton_L
#define KEY_R HidNpadButton_R
#define KEY_ZL HidNpadButton_ZL
#define KEY_ZR HidNpadButton_ZR
#define KEY_PLUS HidNpadButton_Plus
#define KEY_MINUS HidNpadButton_Minus
#define KEY_DUP HidNpadButton_Up
#define KEY_DDOWN HidNpadButton_Down
#define KEY_DLEFT HidNpadButton_Left
#define KEY_DRIGHT HidNpadButton_Right
#define KEY_SL HidNpadButton_AnySL
#define KEY_SR HidNpadButton_AnySR
#define KEY_LSTICK HidNpadButton_StickL
#define KEY_RSTICK HidNpadButton_StickR
#define KEY_UP (HidNpadButton_Up | HidNpadButton_StickLUp | HidNpadButton_StickRUp)
#define KEY_DOWN (HidNpadButton_Down | HidNpadButton_StickLDown | HidNpadButton_StickRDown)
#define KEY_LEFT (HidNpadButton_Left | HidNpadButton_StickLLeft | HidNpadButton_StickRLeft)
#define KEY_RIGHT (HidNpadButton_Right | HidNpadButton_StickLRight | HidNpadButton_StickRRight)

/**
 * @brief Ultrahand-Overlay Input Macros
 *
 * This block of code defines macros for handling input in the Ultrahand-Overlay project.
 * These macros simplify the mapping of input events to corresponding button keys and
 * provide aliases for touch and joystick positions.
 *
 * The macros included in this block are:
 *
 * - `touchPosition`: An alias for a constant `HidTouchState` pointer.
 * - `touchInput`: An alias for `&touchPos`, representing touch input.
 * - `JoystickPosition`: An alias for `HidAnalogStickState`, representing joystick input.
 *
 * These macros are utilized within the Ultrahand-Overlay project to manage and interpret
 * user input, including touch and joystick events.
 */
#define touchPosition const HidTouchState
#define touchInput &touchPos
#define JoystickPosition HidAnalogStickState


/**
 * @brief Ultrahand-Overlay Configuration Paths
 *
 * This block of code defines string variables for various configuration and directory paths
 * used in the Ultrahand-Overlay project. These paths include:
 *
 * - `packageFileName`: The name of the package file ("package.ini").
 * - `configFileName`: The name of the configuration file ("config.ini").
 * - `settingsPath`: The base path for Ultrahand settings ("sdmc:/config/ultrahand/").
 * - `settingsConfigIniPath`: The full path to the Ultrahand settings configuration file.
 * - `packageDirectory`: The base directory for packages ("sdmc:/switch/.packages/").
 * - `overlayDirectory`: The base directory for overlays ("sdmc:/switch/.overlays/").
 * - `teslaSettingsConfigIniPath`: The full path to the Tesla settings configuration file.
 *
 * These paths are used within the Ultrahand-Overlay project to manage configuration files
 * and directories.
 */
const std::string packageFileName = "package.ini";
const std::string configFileName = "config.ini";
const std::string settingsPath = "sdmc:/config/ultrahand/";
const std::string settingsConfigIniPath = settingsPath + configFileName;
const std::string packageDirectory = "sdmc:/switch/.packages/";
const std::string overlayDirectory = "sdmc:/switch/.overlays/";
const std::string teslaSettingsConfigIniPath = "sdmc:/config/tesla/"+configFileName;
const std::string overlaysIniFilePath = settingsPath + "overlays.ini";
const std::string packagesIniFilePath = settingsPath + "packages.ini";





/**
 * @brief Copy Tesla key combo to Ultrahand settings.
 *
 * This function retrieves the key combo from Tesla settings and copies it to Ultrahand settings.
 */
void copyTeslaKeyComboToUltrahand() {
    std::string keyCombo;
    std::map<std::string, std::map<std::string, std::string>> parsedData;
    
    if (isFileOrDirectory(teslaSettingsConfigIniPath)) {
        parsedData = getParsedDataFromIniFile(teslaSettingsConfigIniPath);
        if (parsedData.count("tesla") > 0) {
            auto& teslaSection = parsedData["tesla"];
            if (teslaSection.count("key_combo") > 0) {
                keyCombo = teslaSection["key_combo"];
            }
        }
    }
    
    if (!keyCombo.empty()){
        if (isFileOrDirectory(settingsConfigIniPath)) {
            parsedData = getParsedDataFromIniFile(settingsConfigIniPath);
            if (parsedData.count("ultrahand") > 0) {
                auto& ultrahandSection = parsedData["ultrahand"];
                if (ultrahandSection.count("key_combo") == 0) {
                    // Write the key combo to the destination file
                    setIniFileValue(settingsConfigIniPath, "ultrahand", "key_combo", keyCombo);
                }
            }
        }
    }
    tsl::impl::parseOverlaySettings();
}



/**
 * @brief Ultrahand-Overlay Protected Folders
 *
 * This block of code defines two vectors containing paths to protected folders used in the
 * Ultrahand-Overlay project. These folders are designated as protected to prevent certain
 * operations that may pose security risks.
 *
 * The two vectors include:
 *
 * - `protectedFolders`: Paths to standard protected folders.
 * - `ultraProtectedFolders`: Paths to ultra protected folders with stricter security.
 *
 * These protected folder paths are used within the Ultrahand-Overlay project to enforce
 * safety conditions and ensure that certain operations are not performed on sensitive
 * directories.
 */
const std::vector<std::string> protectedFolders = {
    "sdmc:/Nintendo/",
    "sdmc:/emuMMC/",
    "sdmc:/atmosphere/",
    "sdmc:/bootloader/",
    "sdmc:/switch/",
    "sdmc:/config/",
    "sdmc:/"
};
const std::vector<std::string> ultraProtectedFolders = {
    "sdmc:/Nintendo/",
    "sdmc:/emuMMC/"
};

/**
 * @brief Check if a path contains dangerous combinations.
 *
 * This function checks if a given path contains patterns that may pose security risks.
 *
 * @param patternPath The path to check.
 * @return True if the path contains dangerous combinations, otherwise false.
 */
bool isDangerousCombination(const std::string& patternPath) {
    // List of obviously dangerous patterns
    const std::vector<std::string> dangerousCombinationPatterns = {
        "*",         // Deletes all files/directories in the current directory
        "*/"         // Deletes all files/directories in the current directory
    };

    // List of obviously dangerous patterns
    const std::vector<std::string> dangerousPatterns = {
        "..",     // Attempts to traverse to parent directories
        "~"       // Represents user's home directory, can be dangerous if misused
    };

    // Check if the patternPath is an ultra protected folder
    for (const std::string& ultraProtectedFolder : ultraProtectedFolders) {
        if (patternPath.find(ultraProtectedFolder) == 0) {
            return true; // Pattern path is an ultra protected folder
        }
    }

    // Check if the patternPath is a protected folder
    for (const std::string& protectedFolder : protectedFolders) {
        if (patternPath == protectedFolder) {
            return true; // Pattern path is a protected folder
        }

        // Check if the patternPath starts with a protected folder and includes a dangerous pattern
        if (patternPath.find(protectedFolder) == 0) {
            std::string relativePath = patternPath.substr(protectedFolder.size());

            // Split the relativePath by '/' to handle multiple levels of wildcards
            std::vector<std::string> pathSegments;
            std::string pathSegment;

            for (char c : relativePath) {
                if (c == '/') {
                    if (!pathSegment.empty()) {
                        pathSegments.push_back(pathSegment);
                        pathSegment.clear();
                    }
                } else {
                    pathSegment += c;
                }
            }

            if (!pathSegment.empty()) {
                pathSegments.push_back(pathSegment);
            }

            for (const std::string& pathSegment : pathSegments) {
                // Check if the pathSegment includes a dangerous pattern
                for (const std::string& dangerousPattern : dangerousPatterns) {
                    if (pathSegment.find(dangerousPattern) != std::string::npos) {
                        return true; // Pattern path includes a dangerous pattern
                    }
                }
            }
        }

        // Check if the patternPath is a combination of a protected folder and a dangerous pattern
        for (const std::string& dangerousPattern : dangerousCombinationPatterns) {
            if (patternPath == protectedFolder + dangerousPattern) {
                return true; // Pattern path is a protected folder combined with a dangerous pattern
            }
        }
    }

    // Check if the patternPath is a dangerous pattern
    if (patternPath.find("sdmc:/") == 0) {
        std::string relativePath = patternPath.substr(6); // Remove "sdmc:/"

        // Split the relativePath by '/' to handle multiple levels of wildcards
        std::vector<std::string> pathSegments;
        std::string pathSegment;

        for (char c : relativePath) {
            if (c == '/') {
                if (!pathSegment.empty()) {
                    pathSegments.push_back(pathSegment);
                    pathSegment.clear();
                }
            } else {
                pathSegment += c;
            }
        }

        if (!pathSegment.empty()) {
            pathSegments.push_back(pathSegment);
        }

        for (const std::string& pathSegment : pathSegments) {
            // Check if the pathSegment includes a dangerous pattern
            for (const std::string& dangerousPattern : dangerousPatterns) {
                if (pathSegment == dangerousPattern) {
                    return true; // Pattern path is a dangerous pattern
                }
            }
        }
    }

    // Check if the patternPath includes a wildcard at the root level
    if (patternPath.find(":/") != std::string::npos) {
        std::string rootPath = patternPath.substr(0, patternPath.find(":/") + 2);
        if (rootPath.find('*') != std::string::npos) {
            return true; // Pattern path includes a wildcard at the root level
        }
    }

    // Check if the provided path matches any dangerous patterns
    for (const std::string& pattern : dangerousPatterns) {
        if (patternPath.find(pattern) != std::string::npos) {
            return true; // Path contains a dangerous pattern
        }
    }

    return false; // Pattern path is not a protected folder, a dangerous pattern, or includes a wildcard at the root level
}


/**
 * @brief Interpret and execute a list of commands.
 *
 * This function interprets and executes a list of commands based on their names and arguments.
 *
 * @param commands A list of commands, where each command is represented as a vector of strings.
 */
void interpretAndExecuteCommand(const std::vector<std::vector<std::string>>& commands) {
    std::string commandName, jsonPath, sourcePath, destinationPath, desiredSection, desiredKey, desiredNewKey, desiredValue, offset, customPattern, hexDataToReplace, hexDataReplacement, fileUrl, occurrence;
    
    for (auto& unmodifiedCommand : commands) {
        
        // Check the command and perform the appropriate action
        if (unmodifiedCommand.empty()) {
            // Empty command, do nothing
            continue;
        }

        // Get the command name (first part of the command)
        commandName = unmodifiedCommand[0];
        //logMessage(commandName);
        //logMessage(command[1]);
        
        
        std::vector<std::string> command;
        
        // Modify the command to replace {json_data} placeholder if jsonPath is available
        if (!jsonPath.empty()) {
            std::vector<std::string> modifiedCommand;
            for (const std::string& commandArg : unmodifiedCommand) {
                if (commandArg.find("{json_data(") != std::string::npos) {
                    // Create a copy of the string and modify it
                    std::string modifiedArg = commandArg;
                    modifiedArg = replaceJsonSourcePlaceholder(modifiedArg, jsonPath);
                    // Use modifiedArg as needed
                    modifiedCommand.push_back(modifiedArg);
                } else {
                    modifiedCommand.push_back(commandArg);
                }
            }
            command = modifiedCommand;
        } else {
            command = unmodifiedCommand;
        }
        
        
        
        if (commandName == "json_data") {
            if (command.size() >= 2) {
                jsonPath = preprocessPath(command[1]);
            }
        } else if (commandName == "make" || commandName == "mkdir") {
            // Delete command
            if (command.size() >= 2) {
                sourcePath = preprocessPath(command[1]);
                createDirectory(sourcePath);
            }

            // Perform actions based on the command name
        } else if (commandName == "copy" || commandName == "cp") {
            // Copy command
            if (command.size() >= 3) {
                sourcePath = preprocessPath(command[1]);
                destinationPath = preprocessPath(command[2]);
                
                
                if (sourcePath.find('*') != std::string::npos) {
                    // Delete files or directories by pattern
                    copyFileOrDirectoryByPattern(sourcePath, destinationPath);
                } else {
                    copyFileOrDirectory(sourcePath, destinationPath);
                }
                
            }
        } else if (commandName == "mirror_copy" || commandName == "mirror_cp") {
            // Copy command
            if (command.size() >= 2) {
                sourcePath = preprocessPath(command[1]);
                if (command.size() >= 3) {
                    destinationPath = preprocessPath(command[2]);
                    mirrorCopyFiles(sourcePath, destinationPath);
                } else {
                    mirrorCopyFiles(sourcePath);
                }
            }
        } else if (commandName == "delete" || commandName == "del") {
            // Delete command
            if (command.size() >= 2) {
                sourcePath = preprocessPath(command[1]);
                if (!isDangerousCombination(sourcePath)) {
                    if (sourcePath.find('*') != std::string::npos) {
                        // Delete files or directories by pattern
                        deleteFileOrDirectoryByPattern(sourcePath);
                    } else {
                        deleteFileOrDirectory(sourcePath);
                    }
                }
            }
        } else if (commandName == "mirror_delete" || commandName == "mirror_del") {
            if (command.size() >= 2) {
                sourcePath = preprocessPath(command[1]);
                if (command.size() >= 3) {
                    destinationPath = preprocessPath(command[2]);
                    mirrorDeleteFiles(sourcePath, destinationPath);
                } else {
                    mirrorDeleteFiles(sourcePath);
                }
            }
        } else if (commandName == "rename" || commandName == "move" || commandName == "mv") {
            // Rename command
            if (command.size() >= 3) {

                sourcePath = preprocessPath(command[1]);
                destinationPath = preprocessPath(command[2]);
                //logMessage("sourcePath: "+sourcePath);
                //logMessage("destinationPath: "+destinationPath);
                
                if (!isDangerousCombination(sourcePath)) {
                    if (sourcePath.find('*') != std::string::npos) {
                        // Move files by pattern
                        moveFilesOrDirectoriesByPattern(sourcePath, destinationPath);
                    } else {
                        // Move single file or directory
                        moveFileOrDirectory(sourcePath, destinationPath);
                    }
                } else {
                    //logMessage( "Dangerous combo.");
                }
            } else {
                //logMessage( "Invalid move command.");
                //std::cout << "Invalid move command. Usage: move <source_path> <destination_path>" << std::endl;
            }

        } else if (commandName == "set-ini-val" || commandName == "set-ini-value") {
            // Edit command
            if (command.size() >= 5) {
                sourcePath = preprocessPath(command[1]);

                desiredSection = removeQuotes(command[2]);
                desiredKey = removeQuotes(command[3]);

                for (size_t i = 4; i < command.size(); ++i) {
                    desiredValue += command[i];
                    if (i < command.size() - 1) {
                        desiredValue += " ";
                    }
                }

                setIniFileValue(sourcePath.c_str(), desiredSection.c_str(), desiredKey.c_str(), desiredValue.c_str());
            }
        } else if (commandName == "set-ini-key") {
            // Edit command
            if (command.size() >= 5) {
                sourcePath = preprocessPath(command[1]);

                desiredSection = removeQuotes(command[2]);
                desiredKey = removeQuotes(command[3]);

                for (size_t i = 4; i < command.size(); ++i) {
                    desiredNewKey += command[i];
                    if (i < command.size() - 1) {
                        desiredNewKey += " ";
                    }
                }

                setIniFileKey(sourcePath.c_str(), desiredSection.c_str(), desiredKey.c_str(), desiredNewKey.c_str());
            }
        } else if (commandName == "hex-by-offset") {
            // Edit command
            if (command.size() >= 4) {
                sourcePath = preprocessPath(command[1]);

                offset = removeQuotes(command[2]);
                hexDataReplacement = removeQuotes(command[3]);

                hexEditByOffset(sourcePath.c_str(), offset.c_str(), hexDataReplacement.c_str());
            }
        } else if (commandName == "hex-by-custom-offset") {
            // Edit command
            if (command.size() >= 5) {
                sourcePath = preprocessPath(command[1]);
                customPattern = removeQuotes(command[2]);
                offset = removeQuotes(command[3]);
                hexDataReplacement = removeQuotes(command[4]);

                hexEditByCustomOffset(sourcePath.c_str(), customPattern.c_str(), offset.c_str(), hexDataReplacement.c_str());
            }
        } else if (commandName == "hex-by-swap") {
            // Edit command - Hex data replacement with occurrence
            if (command.size() >= 4) {
                sourcePath = preprocessPath(command[1]);

                hexDataToReplace = removeQuotes(command[2]);
                hexDataReplacement = removeQuotes(command[3]);

                if (command.size() >= 5) {
                    occurrence = removeQuotes(command[4]);
                    hexEditFindReplace(sourcePath, hexDataToReplace, hexDataReplacement, occurrence);
                } else {
                    hexEditFindReplace(sourcePath, hexDataToReplace, hexDataReplacement);
                }
            }
        } else if (commandName == "hex-by-string") {
            // Edit command - Hex data replacement with occurrence
            if (command.size() >= 4) {
                sourcePath = preprocessPath(command[1]);

                hexDataToReplace = asciiToHex(removeQuotes(command[2]));
                hexDataReplacement = asciiToHex(removeQuotes(command[3]));
                //logMessage("hexDataToReplace: "+hexDataToReplace);
                //logMessage("hexDataReplacement: "+hexDataReplacement);
                
                // Fix miss-matched string sizes
                if (hexDataReplacement.length() < hexDataToReplace.length()) {
                    // Pad with spaces at the end
                    hexDataReplacement += std::string(hexDataToReplace.length() - hexDataReplacement.length(), '\0');
                } else if (hexDataReplacement.length() > hexDataToReplace.length()) {
                    // Add spaces to hexDataToReplace at the far right end
                    hexDataToReplace += std::string(hexDataReplacement.length() - hexDataToReplace.length(), '\0');
                }
                
                if (command.size() >= 5) {
                    occurrence = removeQuotes(command[4]);
                    hexEditFindReplace(sourcePath, hexDataToReplace, hexDataReplacement, occurrence);
                } else {
                    hexEditFindReplace(sourcePath, hexDataToReplace, hexDataReplacement);
                }
            }
        } else if (commandName == "hex-by-decimal") {
            // Edit command - Hex data replacement with occurrence
            if (command.size() >= 4) {
                sourcePath = preprocessPath(command[1]);

                hexDataToReplace = decimalToHex(removeQuotes(command[2]));
                hexDataReplacement = decimalToHex(removeQuotes(command[3]));
                //logMessage("hexDataToReplace: "+hexDataToReplace);
                //logMessage("hexDataReplacement: "+hexDataReplacement);

                if (command.size() >= 5) {
                    occurrence = removeQuotes(command[4]);
                    hexEditFindReplace(sourcePath, hexDataToReplace, hexDataReplacement, occurrence);
                } else {
                    hexEditFindReplace(sourcePath, hexDataToReplace, hexDataReplacement);
                }
            }
        } else if (commandName == "hex-by-rdecimal") {
            // Edit command - Hex data replacement with occurrence
            if (command.size() >= 4) {
                sourcePath = preprocessPath(command[1]);

                hexDataToReplace = decimalToReversedHex(removeQuotes(command[2]));
                hexDataReplacement = decimalToReversedHex(removeQuotes(command[3]));
                //logMessage("hexDataToReplace: "+hexDataToReplace);
                //logMessage("hexDataReplacement: "+hexDataReplacement);

                if (command.size() >= 5) {
                    occurrence = removeQuotes(command[4]);
                    hexEditFindReplace(sourcePath, hexDataToReplace, hexDataReplacement, occurrence);
                } else {
                    hexEditFindReplace(sourcePath, hexDataToReplace, hexDataReplacement);
                }
            }
        } else if (commandName == "download") {
            // Edit command - Hex data replacement with occurrence
            if (command.size() >= 3) {
                fileUrl = preprocessUrl(command[1]);
                destinationPath = preprocessPath(command[2]);
                logMessage("fileUrl: "+fileUrl);
                downloadFile(fileUrl, destinationPath);
            }
        } else if (commandName == "unzip") {
            // Edit command - Hex data replacement with occurrence
            if (command.size() >= 3) {
                sourcePath = preprocessPath(command[1]);
                destinationPath = preprocessPath(command[2]);
                unzipFile(sourcePath, destinationPath);
            }
        } else if (commandName == "reboot") {
            // Reboot command
            splExit();
            fsdevUnmountAll();
            spsmShutdown(SpsmShutdownMode_Reboot);
        } else if (commandName == "shutdown") {
            // Reboot command
            splExit();
            fsdevUnmountAll();
            spsmShutdown(SpsmShutdownMode_Normal);
        }
    }
}
