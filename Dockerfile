FROM dea82/avr-gcc-docker:1.3.0
RUN apk add --no-cache doxygen graphviz
CMD bash
