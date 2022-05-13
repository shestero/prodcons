#include <iostream>

#include "fastout.hpp"      // small ostream wrapper to decrese the mess during the mulithread output
#include "wsource.hpp"      // source of words from command line (pull)
#include "fatol.hpp"        // small global procedure to convert char-words int long int
#include "chunker.hpp"      // group the data stream in chunks (push)
#include "tsqueue.hpp"      // thread-safe FIFO buffer with limits and End-Of-Stream
#include "prodcons.hpp"     // multithread producer-consumer
#include "solver.hpp"       // "business logic"

using namespace std;

int main(int argc, char *argv[])
{
    // Intro
    const char* app = "Test task Consumer-Producer";
    cout << "BEGIN\t" << app << endl;

    // Define source of words - from (argc,argv)
    words_source source(argc,argv);

    // solver of math. Results are text stream:
    solver sink(cout);

    // We are to group intput data by 3 values before feed it to solver.
    // Note: An empty optional means failing to convert the corresponding word into a long int number;
    // in this case we are to skip all 3 values.
    chunker<optional<long>,3> feeder(
        [&sink](const optional<long> *triplet) {
            if ( all_of(triplet,triplet+3,[](auto const &op) { return op.has_value(); }) ) {
                sink(triplet[0].value(),triplet[1].value(),triplet[2].value());
            } else {
                fastout(cerr) << "ERROR\tWrong input data. Skip 3 words";
            }
        }
    );

    // Define thread safe buffer of optional<long>. Under the cover there will be nested optional.
    queue_with_end< optional<long> > buffer;

    // worker pipe
    // Note the nested optional comes from source.
    // if the top level optional is nullopt then there is no more data in source;
    // if the second level optional is nullopt then there is a converting problem in the corresponding word.
    prod_cons< optional<long> > pipeline(
        buffer,
        (function< optional< optional<long> >() >) [&source]() {
            auto opWord = source();
            if (!opWord.has_value())
                return optional< optional<long> >();
            auto opLong = word2long(opWord.value());
            if (!opLong.has_value()) {
                fastout(cerr) << "ERROR\tCannot convert " << opWord.value() << " to long!";
            }
            return optional< optional<long> >(opLong);
         }, feeder
    );

    // Warning if unused extra input
    if (!feeder.empty()) {
        cerr << "WARNING\tLast " << feeder.count()
             << " source word(s) is/are ignored!" << endl;
    }

    // Farewell
    cout << "END\t" << app << endl;
    return 0;
}
