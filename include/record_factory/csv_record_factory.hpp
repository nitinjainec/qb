#ifndef __CSV_RECORD_FACTORY_HPP__
#define __CSV_RECORD_FACTORY_HPP__

#include <interface.hpp>
#include <record/quote.hpp>
#include <record/trade.hpp>
#include <record/signal.hpp>

/*
  CSV Record factory class to create Record based on fields
*/
typedef std::vector <std::string> CSVRecordFactoryType;
class CSVRecordFactory : public IRecordFactory <CSVRecordFactoryType> {

  RecordPtr createQuote (const std::vector<std::string> &fields) {
    assert (fields.size () == 6);
    return RecordPtr (new Quote (fields[0],
				 fields[1],
				 std::stod (fields[2]),
				 std::stod (fields[3]),
				 std::stoi (fields[4]),
				 std::stoi (fields[5])));
  }

  RecordPtr createTrade (const std::vector<std::string> &fields) {
    assert (fields.size () == 3 || fields.size () == 4);
    return (fields.size () == 3 || fields[3].size () == 0)
      ? RecordPtr (new Trade (fields[0],
			      fields[1],
			      std::stod (fields[2])))
      : RecordPtr (new Trade (fields[0],
			      fields[1],
			      std::stod (fields[2]),
			      fields[3][0]));
  }

  RecordPtr createSignal (const std::vector<std::string> &fields) {
    assert (fields.size () == 4);
    return RecordPtr (new Signal (fields[0],
				  fields[1],
				  std::stod (fields[2]),
				  std::stoi (fields[3])));
  }

public:
  /* Returns true if Record can be created with given fields */
  bool canCreateRecord (const std::vector<std::string> &fields) {
    return (fields.size () == 3
	    || fields.size () == 4
	    || fields.size () == 6);
  }

  /* Creates the Record with given fields */
  RecordPtr createRecord (const std::vector<std::string> &fields) {
    assert (canCreateRecord (fields));

    if (fields.size () == 6)
      return createQuote (fields);

    if (fields.size () == 3
	|| (fields[3].size () == 1 && (fields[3][0] < '0' || fields[3][0] > '9'))
	|| fields[3].size () == 0)
      return createTrade (fields);

    return createSignal (fields);
  }
};
typedef IRecordFactory<CSVRecordFactoryType>::Ptr CSVRecordFactoryPtr;

#endif // __CSV_RECORD_FACTORY_HPP__
