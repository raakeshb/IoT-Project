# -*- coding: utf-8 -*-
import os,sys
from pyspark import SparkContext
from pyspark.streaming import StreamingContext
from pyspark.streaming.kafka import KafkaUtils
import happybase
from datetime import datetime
import json

j=0
sc = SparkContext("local","first app")
ssc = StreamingContext(sc,10)
brokers = "localhost:9092"
topic = "yui"
kafkaParams = {"metadata.broker.list": "localhost:9092",
                       "auto.offset.reset": "smallest"}
kvs = KafkaUtils.createDirectStream(ssc, [topic],kafkaParams)
kvs.pprint()
print("IM here")

#lines = kvs.map(lambda x: x[1])
#counts = lines.flatMap(lambda line:line.split(","))
parsed = kvs.map(lambda v: json.loads(v[1]))
#counts = lines.map(lambda x:(x.split(":")[0],x.split(":")[1]))
#counts.pprint()

connection = happybase.Connection('localhost', 9090)
connection.open()
#connection.create_table(
#	'hh',
#	{
#	'cf1':dict()
#	}
#	)
print("I have connected>>>")
table = connection.table('espreal')
print(table)
parsed.foreachRDD(lambda rdd:
	SaveToHBase(rdd)


	

	)
print("here")
def SaveToHBase(rdd):
    j=0
    print("in func")
    if not rdd.isEmpty():
        print("records captured")
        
        for line in rdd.collect():
             print("in for loop")
             table.put('j',{'topics:temperature':str(line)})
             j=j+1
             print(line)
    else:
    	print("No-dataa........................................")
    

ssc.start()
ssc.awaitTermination()
