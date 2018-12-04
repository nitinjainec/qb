
#include "logger.hpp"
#include "parser.hpp"
#include "processor.hpp"
#include "reader.hpp"
#include "writer.hpp"

using namespace std;

void test () {
  IReaderPtr reader (new CSVReader ("qb.csv"));
  IParserPtr parser (new CSVParser (reader));
  RecordPtr record = parser->nextRecord ();
  const ByteBuffer &buffer = record->toByteBuffer ();
  Quote q (buffer);

  cout << "\n\n------------------ \n";
  cout << q.time.toString () << "\n";
  cout << q.symbol << "\n";
  cout << q.bid << "\n";
  cout << q.ask << "\n";
  cout << q.bsize << "\n";
  cout << q.asize << "\n";
  cout << " ------------------ \n\n";
  
  const char *ch = buffer.c_str ();


  RecordType rt;
  memcpy (&rt, ch, sizeof (rt));
  ch += sizeof (rt);
  assert (rt == QUOTE);

  char sym[5];
  memcpy (sym, ch, 5);
  sym [4] = '\0';
  ch += 5;
  cout << sym << "\n";
  
  char chd[24];
  memcpy (chd, ch, 23);
  chd [23] = '\0';
  ch += 23;
  cout << chd << "\n";

  double bid;
  memcpy (&bid, ch, sizeof (bid));
  ch += sizeof (bid);
  cout << bid << "\n";

  double ask;
  memcpy (&ask, ch, sizeof (ask));
  ch += sizeof (ask);
  cout << ask << "\n";

  
  uint32_t bsize;
  memcpy (&bsize, ch, sizeof (bsize));
  ch += sizeof (bsize);
  cout << bsize << "\n";

  uint32_t asize;
  memcpy (&asize, ch, sizeof (asize));
  ch += sizeof (asize);
  cout << asize << "\n";

  
  /*
  

    buffer.append (time.toByteBuffer ());
    buffer.append (symbol, sizeof (symbol));
    buffer.append ((char *)&bid, sizeof (bid));
    buffer.append ((char *)&ask, sizeof (ask));
    buffer.append ((char *)&bsize, sizeof (bsize));
    buffer.append ((char *)&asize, sizeof (asize));
    return buffer;
  */
  

  //assert (*(reinterpret_cast<const RecordType *>(ch)) == QUOTE);
}

class MockReader : public IReader {
  bool _eod;
public:
  
  MockReader () : _eod (false) { }
  ByteBuffer getData () {
    _eod = true;
    return "2013-04-12 17:00:00.006,ZNM3,132.9531,132.9688,697,3493";
  }
  bool eod () { return _eod; }
};

void test2 () {
  IReaderPtr reader (new MockReader ());
  IParserPtr parser (new CSVParser (reader));

  assert (!parser->eor ());
  RecordPtr record_a = parser->nextRecord ();
  assert (parser->eor ());

  ByteBuffer buffer = record_a->toByteBuffer ();

  assert (RecordFactory::canCreate (buffer));
  RecordPtr record_b = RecordFactory::create (buffer);

  assert (record_a->toString () == record_b->toString ());
  std::cout << "Quote test passed";
}

int main () {
  Logger::create ();
    
  //test ();
  test2 ();
}
