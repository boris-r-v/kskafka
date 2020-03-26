#ifndef _kafka_server_hpp_
#define _kafka_server_hpp_

#include <serverBase.hh>
#include <mongoose/mongoosemm.h>
#include <ksa/mut.hpp>
#include <unordered_map>

namespace kafka{
    class server: public kafka::serverBase, public mongoose::MongooseServer, public ksa::Thread
    {
	public:
	    server( const ksa::ItemNode*, const ksa::ItemContent*  );
	    virtual ~server();
	
	public:
	    virtual void configure__();
	    virtual void activate__();
	    virtual void applyAttrs();
	    virtual void run() ;

	private:
	    virtual bool on_event(	mongoose::ServerHandlingEvent _event,
					mongoose::MongooseConnection &_connection,
					mongoose::MongooseRequest const &_request,
					mongoose::MongooseResponse &_response);

	    void handle_get( mongoose::MongooseRequest const &_request, mongoose::MongooseResponse &_response );
	    void handle_post( mongoose::MongooseRequest const &_request, mongoose::MongooseResponse &_response );
	    void update_inner_storage(std::string const& _pl);
	    
	    std::unordered_map<int, std::string> map_;

    };
}

#endif //_kafka_server_hpp_
