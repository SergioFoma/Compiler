CXX = g++

BUILD_DIR = Build

FLAGS =-Wshadow -Winit-self -Wredundant-decls -Wcast-align \
	-Wundef -Wfloat-equal -Winline -Wunreachable-code -Wmissing-declarations\
	-Wmissing-include-dirs -Wswitch-enum -Wswitch-default -Weffc++ -Wmain\
	-Wextra -Wall -g -pipe -fexceptions -Wcast-qual -Wconversion -Wctor-dtor-privacy\
	-Wempty-body -Wformat-security -Wformat=2 -Wignored-qualifiers -Wlogical-op -Wno-missing-field-initializers\
	-Wnon-virtual-dtor -Woverloaded-virtual -Wpointer-arith -Wsign-promo -Wstack-usage=8192 -Wstrict-aliasing\
	-Wstrict-null-sentinel -Wtype-limits -Wwrite-strings -Werror=vla -D_DEBUG -D_EJUDGE_CLIENT_SIDE

SPU_FOLDER_NAME = Processor
SPU_EXE_NAME = $(addprefix $(SPU_FOLDER_NAME)/, $(BUILD_DIR) )/Result.exe  # Processor/Build/Result.exe
SPU_SOURCES = main.cpp stack.cpp spuCheckError.cpp stackCheckError.cpp paint.cpp spuLoop.cpp stringFunction.cpp softProcessor.cpp
SPU_OBJECTS = $(SPU_SOURCES:.cpp=.obj)

ASM_FOLDER_NAME = ASM
ASM_EXE_NAME = $(addprefix $(ASM_FOLDER_NAME)/, $(BUILD_DIR) )/Result.exe
ASM_SOURCES = main.cpp paint.cpp assembleCode.cpp stringFunction.cpp
ASM_OBJECTS = $(ASM_SOURCES:.cpp=.obj)

SPU_OBJECTS_IN_SOURCE = $(addprefix $(SPU_FOLDER_NAME)/src/, $(SPU_OBJECTS))  # Processor/src/main.obj
ASM_OBJECTS_IN_SOURCE = $(addprefix $(ASM_FOLDER_NAME)/src/, $(ASM_OBJECTS))  # ASM/src/main.obj

.PHONY: all clean

all: $(SPU_EXE_NAME) $(ASM_EXE_NAME)

############################# SPU #################################################################
$(SPU_EXE_NAME): SPU_makeBuild $(SPU_OBJECTS_IN_SOURCE)
	$(CXX) $(FLAGS) $(addprefix $(SPU_FOLDER_NAME)/$(BUILD_DIR)/, $(SPU_OBJECTS_IN_SOURCE)) -o $(SPU_EXE_NAME)

$(SPU_OBJECTS_IN_SOURCE) : %.obj: %.cpp
	$(CXX) $(FLAGS) -I ./$(SPU_FOLDER_NAME)/include -c ./$^ -o ./$(SPU_FOLDER_NAME)/$(BUILD_DIR)/$@

##################################  ASM	 ###################################################################
$(ASM_EXE_NAME): ASM_makeBuild $(ASM_OBJECTS_IN_SOURCE)
	$(CXX) $(FLAGS) $(addprefix $(ASM_FOLDER_NAME)/$(BUILD_DIR)/, $(ASM_OBJECTS_IN_SOURCE)) -o $(ASM_EXE_NAME)

$(ASM_OBJECTS_IN_SOURCE) : %.obj: %.cpp
	$(CXX) $(FLAGS) -I ./$(ASM_FOLDER_NAME)/include -c ./$^ -o ./$(ASM_FOLDER_NAME)/$(BUILD_DIR)/$@

##############################################################################################################

SPU_makeBuild:
	mkdir -p ./$(SPU_FOLDER_NAME)/$(BUILD_DIR)/$(SPU_FOLDER_NAME)/src
ASM_makeBuild:
	mkdir -p ./$(ASM_FOLDER_NAME)/$(BUILD_DIR)/$(ASM_FOLDER_NAME)/src
clean:
	rm -rf ./Processor/$(BUILD_DIR)
	rm -rf ./ASM/$(BUILD_DIR)
	rm -rf BYTE-CODE.txt

#Processor:
#	FOLDER_NAME = Processor
#	SOURCES = main.cpp stack.cpp checkError.cpp paint.cpp SPULoop.cpp onegin.cpp softProcessor.cpp
#ASM:
#	FOLDER_NAME = ASM
#	SOURCES = main.cpp paint.cpp assembleCode.cpp onegin.cpp
