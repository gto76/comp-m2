FROM gcc

RUN mkdir -p /opt/comp-m2
WORKDIR /opt/comp-m2
COPY . /opt/comp-m2

RUN make

ENTRYPOINT ["./comp"]
