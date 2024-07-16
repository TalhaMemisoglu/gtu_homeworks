# Define variables
SRC_DIR := Hmw08
BUILD_DIR := build
DOC_DIR := docs
PACKAGE_NAME := Hmw08

# Define source files and classes
SOURCES := $(wildcard $(SRC_DIR)/*.java)
CLASSES := $(patsubst $(SRC_DIR)/%.java,$(BUILD_DIR)/%.class,$(SOURCES))

# Define the main class
MAIN_CLASS := Main

# Default target
.PHONY: all
all: clean $(CLASSES)

# Compile Java source files
$(BUILD_DIR)/%.class: $(SRC_DIR)/%.java
	mkdir -p $(BUILD_DIR)
	javac -d $(BUILD_DIR) $<

# Run the main class
.PHONY: run
run:
	java -Xint -cp $(BUILD_DIR) $(PACKAGE_NAME).$(MAIN_CLASS)

# Generate Javadoc
.PHONY: javadoc
javadoc:
	mkdir -p $(DOC_DIR)
	javadoc -d $(DOC_DIR) $(SOURCES)

# Clean Javadoc
.PHONY: clean-javadoc
clean-javadoc:
	rm -rf $(DOC_DIR)

# Clean compiled files
.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)
