# MediaMinerSentiment

A Qt-based C++ application for sentiment analysis using FastText models. This application provides a graphical user interface for analyzing sentiment in text data and generating word clouds.

## Features

- **Sentiment Analysis**: Analyze sentiment of text using pre-trained FastText models
- **Word Cloud Generation**: Create visual word clouds from analyzed text
- **File Processing**: Support for batch processing of text files
- **GUI Interface**: User-friendly Qt-based graphical interface
- **Cross-platform**: Compatible with Windows, macOS, and Linux

## Prerequisites

- Qt 5.12 or higher
- C++17 compatible compiler
- CMake (for building FastText)
- FastText library

## Installation

1. Clone the repository:
```bash
git clone https://github.com/ralharas/MediaMiner.git
cd MediaMiner
```

2. Build the FastText library:
```bash
cd fastText
mkdir build && cd build
cmake ..
make
cd ../..
```

3. Build the application:
```bash
qmake MediaMinerSentiment.pro
make
```

## Usage

1. Launch the application:
```bash
./MediaMinerSentiment
```

2. Use the GUI to:
   - Load text files for analysis
   - Configure sentiment analysis parameters
   - Generate and view word clouds
   - Export results

## Project Structure

- `main.cpp` - Application entry point
- `mainwindow.cpp/h` - Main window implementation
- `page.cpp/h` - Page handling components
- `sentiment_binding.cpp` - FastText integration
- `word.cpp/h` - Word processing utilities
- `WordCloudWidget.cpp/h` - Word cloud visualization
- `fastText/` - FastText library source
- `model/` - Pre-trained models directory
- `tests/` - Unit tests

## Models

The application uses pre-trained FastText models for sentiment analysis:
- `model.bin` - Binary model file
- `model.vec` - Vector representations
- `train.txt` - Training data

## Configuration

The application can be configured through:
- Qt project file (`MediaMinerSentiment.pro`)
- Model parameters in the GUI
- Training data customization

## Development

### Building for Development

```bash
CONFIG += debug qmake MediaMinerSentiment.pro
make
```

### Running Tests

```bash
cd tests
# Run specific test files
```

### Documentation

Generate documentation using Doxygen:
```bash
doxygen Doxyfile
```

Documentation will be generated in the `html/` directory.

## Dependencies

- Qt Core and GUI modules
- FastText library
- C++17 standard library

## Contributing

1. Fork the repository
2. Create a feature branch
3. Make your changes
4. Add tests if applicable
5. Submit a pull request

## License

This project is developed for CS3307B coursework.

## Authors

- Rawad Alhastrani (primary developer)
  
## Support

For issues and questions, please use the GitHub Issues page or contact the development team.

## Acknowledgments

- FastText team for the underlying NLP library
- Qt framework for the GUI components
- CS3307B course instructors and TAs

