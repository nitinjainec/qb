#ifndef __PROCESSOR_HPP__
#define __PROCESSOR_HPP__

#include "interface.hpp"

class CSVToBinaryProcessor : public IProcessor {
  IParserPtr parser;
  std::vector <IWriterPtr> writers;

  void notifyWriters (const RecordPtr &record) {
    for (int i = 0; i < writers.size (); ++i) {
      VLOG ("Notifying writers");
      writers[i]->notify (record);
    }
  }

public:
  CSVToBinaryProcessor (const IParserPtr &parser)
    : parser (parser)
  {
    DLOG ("CSVToBinaryProcessor constructed");
  }

  /* Register writer */
  void registerWriter (const IWriterPtr &writer) {
    writers.push_back (writer);
    DLOG ("Registered writer");
  }

  /* Process the data and notify registered writers */
  void process () {
    DLOG ("started processing csv to binary"); 
    while (!parser->eor ()) {
      VLOG ("Getting next record");
      const RecordPtr &record = parser->nextRecord ();
      VLOG ("Next record received");
      notifyWriters (record);
      VLOG ("Writers notified");
    }
  }
};

#endif
