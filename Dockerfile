FROM dea82/avr-gcc-docker:1.1.0
RUN apk add --no-cache doxygen graphviz
CMD bash
