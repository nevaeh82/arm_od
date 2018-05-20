#ifndef GEOSEARCH_COMMON_H
#define GEOSEARCH_COMMON_H

// defines setter for property
#define SETTER( type, holder, name ) \
	/** @brief Sets value for holder */\
	void name( const type& value ) { holder = value; }

// defines getter for property
#define GETTER( type, holder, name ) \
	/** @brief Gets holder value */\
	type name() { return holder; }\
	type name() const { return holder; }

// defines class readonly property
#define PROPERTY_READ( type, holder, getter ) \
	private: type holder;\
	public: GETTER( type, holder, getter )

// defines class property with setter and getter
#define PROPERTY( type, holder, getter, setter ) \
	PROPERTY_READ( type, holder, getter )\
	SETTER( type, holder, setter )

// defines class readonly property for ActiveQt object
#define AX_PROPERTY_READ( type, holder, getter ) \
	private: type holder;\
	public slots:\
	GETTER( type, holder, getter )

// defines class property for ActiveQt object
#define AX_PROPERTY( type, holder, getter, setter ) \
	AX_PROPERTY_READ( type, holder, getter )\
	SETTER( type, holder, setter )

#endif // GEOSEARCH_COMMON_H
