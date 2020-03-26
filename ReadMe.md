#kskafka
=======

Handle message query via http:


**Request:**

Put data: 
    
    curl -X POST "http://127.0.0.1:8888/v1/item" -H "accept: application/json" -H "Content-Type: application/json" -d "{\"key\": 5, \"value\":  \"decoder\"}"	

Get data: 

    curl -X GET "http://127.0.0.1:8888/v1/item/5" -H "accept: application/json"

**Answer:**

When post json:
	
	http status code: 201

When data for the specified key exists:

	http status code: 200 and posted json
    
When data for the specified key is not available:

	http status code: 404


**Usage**

Install Docker, if you don`t have it

    Visit: https://docs.docker.com/install/    

Get the nessary docker images

    1. docker pull borisrozhkin/ksweb:v3
    2. docker pull spotify/kafka    

Run the contaiers

    1. docker run -d --name kafka -p 2181:2181 -p 9092:9092 --env ADVERTISED_HOST=`docker-machine ip \`docker-machine active\`` --env ADVERTISED_PORT=9092 spotify/kafka
    2. docker run -d --name ks --link kafka:kafka -p 8888:8888 borisrozhkin/ksweb:v3 ksrun /opt/ksa.xml

Use GET and POST requests specified earlier to verify


**Limitations**

    1. Support only specified earlier JSON structure
    2. Data race no safity


**Scripts**

    test/get.sh 	-- get data
    test/post.sh	-- put data

**Usefull commands**

    docker ps	
    docker ps -a
    docker start kafka|ks
    docker stop kafka|ks
    docker exec -it kafka|ks bash
    docker rm container_id
    docker rmi repository:tag


**
