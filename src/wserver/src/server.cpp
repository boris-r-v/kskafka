#include <server.hpp>
#include <ksa/json.hpp>
#include <cppkafka/cppkafka.h>

using namespace cppkafka;

static std::string decode(std::string const &_str);

kafka::server::server(const ksa::ItemNode* _ptr, const ksa::ItemContent* _cntx)
    : kafka::serverBase (_ptr, _cntx)
{
    _in_root = "html";
    _in_port = "8080";
    _in_threads = "1";
}

kafka::server::~server()
{
}

void kafka::server::configure__( )
{ //third stage of activation of element	
}

void kafka::server::activate__( )
{ //first stage of activation of element	

}


void kafka::server::applyAttrs( )
{ //second stage of activation of element	
    kafka::serverBase::applyAttrs( );

    setOption("document_root", _in_root);
    setOption("listening_ports", _in_port);
    setOption("num_threads", _in_threads);

    KSA_LOG << "Embedded kafka-server mongoose-" << MongooseServer::getVersion() << ", starts at port:\"" << _in_port <<"\", with root:\"" << _in_root <<"\" and threads:\"" << _in_threads << "\"" << std::endl; 
    KSA_LOG << "KafkaMQ-server " << _in_kafka << std::endl; 
    MongooseServer::start();
    Thread::start();
}

void kafka::server::run()
{
    // Construct the configuration
    Configuration config = {
        { "metadata.broker.list", _in_kafka },
        { "group.id", getId__() },
	{ "topic.auto.offset.reset", "earliest"},
        { "enable.auto.commit", false }
    };

    Consumer consumer(config);
    consumer.set_assignment_callback([](const TopicPartitionList& partitions) {
        std::cout << "Got assigned: " << partitions << std::endl;
    });
    consumer.set_revocation_callback([](const TopicPartitionList& partitions) {
        std::cout << "Got revoked: " << partitions << std::endl;
    });
    consumer.subscribe({"test"});

    while ( m() )
    {
	Message msg = consumer.poll();
	if (msg) {
            if (msg.get_error()) {
                if (!msg.is_eof()) {
                    std::cout << "[+] Received error notification: " << msg.get_error() << std::endl;
                }
            }
            else {
                if (msg.get_key()) {
                    std::cout << msg.get_key() << " -> " << msg.get_payload() << std::endl;
                }
                //consumer.commit(msg);
		update_inner_storage( msg.get_payload() );

            }
        }
    }
}

void kafka::server::update_inner_storage(std::string const& _pl)
{
    std::istringstream in( _pl );
    ksa::JsonObject message;
    in >> message;
    if ( message.has_key("key") and message.has_key("value") )
	map_.insert( { message["key"].get<int>(), message["value"].get<std::string>() } );	
std::cout << message << std::endl;
}

bool kafka::server::on_event( mongoose::ServerHandlingEvent _event,
			    mongoose::MongooseConnection &_connection,
			    mongoose::MongooseRequest const &_request,
			    mongoose::MongooseResponse &_response)
{
    if(_event == MG_NEW_REQUEST)
    {
	std::string uri = _request.getUri();
	if (debug() )std::cout << "on event uri " << uri << std::endl;

        if( "GET" == _request.getRequestMethod() ) handle_get( _request, _response );
        if( "POST" == _request.getRequestMethod() ) handle_post( _request, _response );
        return true;
    }

    return mongoose::MongooseServer::on_event(_event, _connection, _request, _response);
}

void kafka::server::handle_get( mongoose::MongooseRequest const &_request, mongoose::MongooseResponse &_response )
{

    std::string uri = _request.getUri();
    std::string msk = "/v1/item/";
    size_t ix = uri.find ( msk );
    if ( ix != std::string::npos )
    {
	try
	{
	    int id = std::stoi( uri.substr( ix+msk.size() ) );
	    if (debug() ) std::cout << "ask value at key = "<< id << std::endl;
	    if ( map_.find( id ) != map_.end() )
	    {
	        ksa::JsonObject m;
		m["key"] = id;
		m["value"] = map_[id];
	        std::ostringstream out;
		out << m << "\n";
	        _response.addContent( out.str() );
		_response.setStatus(200);
	    }
	    else 
	        _response.setStatus(400);

	}
	catch (std::exception const& e)
	{
	    std::cerr << "stoi exceptions: " << e.what() << std::endl;
	    _response.setStatus(400);
	}
    }
    else
	_response.setStatus(400);

    _response.setConnectionAlive(false);
    _response.setCacheDisabled();
    _response.setContentType("application/json");
    _response.write();

}

void kafka::server::handle_post( mongoose::MongooseRequest const &_request, mongoose::MongooseResponse &_response )
{
    _response.setStatus(201);
    _response.setConnectionAlive(false);
    _response.setCacheDisabled();
    _response.setContentType("application/json");
    _response.write();

    std::string query = decode(_request.readQueryString());

    Configuration config = {
        { "metadata.broker.list", _in_kafka } //10.0.10.236:9092
    };
    // Create the producer
    Producer producer(config);
    // Produce a message!
    producer.produce(MessageBuilder("test").partition(0).payload(query));
    producer.flush();
}
std::string decode(std::string const &_str)
{
    std::string retval;
    for(std::string::const_iterator itr = _str.begin(), end = _str.end(); itr != end;)
    {
	char c = *itr++;
    	if (c == '%')
    	{
    	    if (itr == end) break;
    	    char hi = *itr++;

    	    if (itr == end) break;
    	    char lo = *itr++;
    	    
	    if (hi >= '0' && hi <= '9') c = hi - '0';
    	    else if (hi >= 'A' && hi <= 'F') c = hi - 'A' + 10;
    	    else if (hi >= 'a' && hi <= 'f') c = hi - 'a' + 10;
    	    else break;

    	    c *= 16;
    	    if (lo >= '0' && lo <= '9') c += lo - '0';
    	    else if (lo >= 'A' && lo <= 'F') c += lo - 'A' + 10;
    	    else if (lo >= 'a' && lo <= 'f') c += lo - 'a' + 10;
    	    else break;
	}
	retval += c;
    }
    return retval;
}

