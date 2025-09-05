CXX      := g++
CXXFLAGS := -Wall -O3 -s -std=c++17

# Detect OS
UNAME_S := $(shell uname -s)
ifeq ($(OS),Windows_NT)
	CXXFLAGS += -static
	EXT      := .exe
else
	EXT      :=
endif

TARGET   := ssp$(EXT)

# Build
all: $(TARGET)

$(TARGET): ssp.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

# Package per‐platform
package: all
ifndef VERSION
	$(error VERSION must be set, e.g. make package VERSION=v1.0.0)
endif
ifeq ($(OS),Windows_NT)
	zip -j ssp-$(VERSION)-windows.zip $(TARGET) LICENSE
else ifeq ($(UNAME_S),Linux)
	tar czf ssp-$(VERSION)-linux.tar.gz $(TARGET) LICENSE
else ifeq ($(UNAME_S),Darwin)
	tar czf ssp-$(VERSION)-macos.tar.gz $(TARGET) LICENSE
else
	@echo "Unsupported OS: $(OS)/$(UNAME_S)"
	exit 1
endif

# Clean
clean:
	rm -f $(TARGET)
ifdef VERSION
	rm -f ssp-$(VERSION)-*.*
endif

.PHONY: all package clean

