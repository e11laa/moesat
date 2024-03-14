
macro(setupCoreAudioSupport)
    if(SMTG_MAC AND XCODE)
        set(SMTG_IOS_DEVELOPMENT_TEAM "" CACHE STRING "Needed for building the InterAppAudio and AUv3 examples for iOS")
        if(NOT SMTG_COREAUDIO_SDK_PATH)
            # Check if the CoreAudio SDK is next to the VST3SDK:
            if(EXISTS "${SDK_ROOT}/../CoreAudio/AudioUnits/AUPublic/AUBase/AUBase.cpp")
                set(SMTG_COREAUDIO_SDK_PATH "${SDK_ROOT}/../CoreAudio")
            else()
                if(EXISTS "${SDK_ROOT}/external.apple.coreaudio/AudioUnits/AUPublic/AUBase/AUBase.cpp")
                    set(SMTG_COREAUDIO_SDK_PATH "${SDK_ROOT}/external.apple.coreaudio")
                endif()
            endif()
        else()
            if(NOT IS_ABSOLUTE ${SMTG_COREAUDIO_SDK_PATH})
                get_filename_component(SMTG_COREAUDIO_SDK_PATH "${SDK_ROOT}/${SMTG_COREAUDIO_SDK_PATH}" ABSOLUTE)
            endif()
            if(NOT EXISTS "${SMTG_COREAUDIO_SDK_PATH}/AudioUnits/AUPublic/AUBase/AUBase.cpp")
                message(FATAL_ERROR "[SMTG] SMTG_COREAUDIO_SDK_PATH is set but does not point to an expected location!")
            endif()
        endif()
        if(SMTG_COREAUDIO_SDK_PATH)
            message(STATUS "[SMTG] SMTG_COREAUDIO_SDK_PATH is set to: " ${SMTG_COREAUDIO_SDK_PATH})
        else()
            message(STATUS "[SMTG] SMTG_COREAUDIO_SDK_PATH is not set. If you need it, please download the COREAUDIO SDK!")
        endif(SMTG_COREAUDIO_SDK_PATH)
    endif(SMTG_MAC AND XCODE)
endmacro(setupCoreAudioSupport)
