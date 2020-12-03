#
# Makefile: A GNU Makefile for building a ray tracer.
#
# Author:   Scott Gigawatt
#
# Version:  9 June 2017
#

#
# Build configuration variables.
#
TARGET    = raytrace
CLEAN     = clean
ALL       = all
SRC_DIR   = src
BIN_DIR   = bin
OBJ_FILES = image.o light.o list.o material.o mem.o model.o object.o plane.o \
            pplane.o fplane.o tplane.o projection.o raytrace.o sphere.o      \
		    psphere.o veclib3d.o main.o
		    # refsphere.o projplane.o illum.o
		    # texplane.o texture.o

OBJECTS   = $(addprefix $(BIN_DIR)/, $(OBJ_FILES))
CFLAGS    = -Wall
LIBS      = -lpthread
CC        = gcc
RM        = rm -vrf
CP        = cp -vrf
MKDIR     = mkdir -vp
DEBUG     = -DDBG_PIX -DDBG_HIT -DDBG_WORLD -DDBG_AMB -DDBG_FIND -DDBG_DIFFUSE
#DEBUG     = -DDBG_AMB -DDBG_DIFFUSE

#
# Targets that are not files (i.e. never up-to-date); these will run every
# time the target is called or required.
#
.PHONY: $(CLEAN)

#
# $(ALL):       The default target for this makefile.  This target builds the
#               raytracer and outputs all files into the $(BIN_DIR) directory.
#
# Dependencies: $(TARGET) - The target executable file.
#
$(ALL): $(TARGET)

#
# $(PEERS):     Creates the number of peers specified in $(NUM_PEERS), by
#               recursively copying the files from the $(BIN_DIR) directory
#               into the $(PEERS) directory. This target is meant to
#               streamline the testing process.
#
# Dependencies: $(TARGET) - The target executable file.
#
# $(PEERS): $(TARGET)
# 	$(RM) $(OBJECTS)
# 	$(MKDIR) $(PEERS)
# 	$(foreach NUM, $(NUM_PEERS), $(CP) $(BIN_DIR) $(PEERS)/$(TARGET)$(NUM);)

#
# $(TARGET):    Creates and outputs the target executable fileinto the
#								$(BIN_DIR) directory.
#
# Dependencies: $(OBJECTS) - The object files to link.
#
$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $(BIN_DIR)/$@
	# $(CP) $(RESOURCES) $(BIN_DIR)


#
# $(BIN_DIR)/%.o: Creates and outputs the individual object files for all of
#                 the associated '$(SRC_DIR)/*.c' files into the $(BIN_DIR)
#									directory.
#
# Dependencies:   $(SRC_DIR)/%.c - The C+ source files.
#                 $(BIN_DIR)     - The object file output directory.
#
$(BIN_DIR)/%.o: $(SRC_DIR)/%.c $(BIN_DIR)
	$(CC) -c $< -o $@

#
# $(BIN_DIR): Creates the $(BIN_DIR) directory for outputting object and
#             binary files.
#
$(BIN_DIR):
	$(MKDIR) $(BIN_DIR)

#
# $(CLEAN): Recursively removes the $(BIN_DIR) directories.
#
$(CLEAN):
	$(RM) $(BIN_DIR)
