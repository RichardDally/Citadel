#include "KafkaClient.h"
#include <spdlog/spdlog.h>

#include <kafka/Properties.h>
#include <kafka/KafkaProducer.h>
#include <kafka/KafkaConsumer.h>
using namespace kafka;
using namespace kafka::clients;
using namespace kafka::clients::producer;


namespace
{
    const char* const brokers = "localhost:9092";
    const char* const topic = "games_v1";
}

namespace Citadel
{
    void Produce()
    {
        const Properties producerProps ({
                                            {"bootstrap.servers",  { brokers }},
                                            {"enable.idempotence", { "true" }},
                                        });

        // Create a producer instance
        KafkaProducer producer(producerProps);

        // Read messages from stdin and produce to the broker
        spdlog::info("% Type message value and hit enter to produce message. (empty line to quit)");

        for (auto line = std::make_shared<std::string>(); std::getline(std::cin, *line); line = std::make_shared<std::string>())
        {
            if (line->empty())
            {
                break;
            }

            // The ProducerRecord doesn't own `line`, it is just a thin wrapper
            auto record = ProducerRecord(topic,
                                         NullKey,
                                         Value(line->c_str(), line->size()));

            // Send the message
            producer.send(record,
                          // The delivery report handler
                          // Note: Here we capture the shared_pointer of `line`,
                          //       which holds the content for `record.value()`.
                          //       It makes sure the memory block is valid until the lambda finishes.
                          [line](const RecordMetadata& metadata, const Error& error) {
                if (!error) {
                    std::cout << "% Message delivered: " << metadata.toString() << std::endl;
                } else {
                    std::cerr << "% Message delivery failed: " << error.message() << std::endl;
                }
            });
        }
    }

    void Consume()
    {
        std::map<std::string, std::string> lol { {"bootstrap.servers", { brokers }} };
        const Properties consumerProps (lol);

        // Create a producer instance
        KafkaConsumer consumer(consumerProps);

        spdlog::info("Subscribing...");
        consumer.subscribe({topic});
        spdlog::info("Subscribed");

        spdlog::info("Polling...");
        auto records = consumer.poll(std::chrono::seconds(5));
        spdlog::info("Polled");
        for (const auto& record: records) {
            // In this example, quit on empty message
            if (record.value().size() == 0)
                break;

            if (!record.error())
            {
                std::cout << "% Got a new message..." << std::endl;
                std::cout << "    Topic    : " << record.topic() << std::endl;
                std::cout << "    Partition: " << record.partition() << std::endl;
                std::cout << "    Offset   : " << record.offset() << std::endl;
                std::cout << "    Timestamp: " << record.timestamp().toString() << std::endl;
                std::cout << "    Headers  : " << toString(record.headers()) << std::endl;
                std::cout << "    Key   [" << record.key().toString() << "]" << std::endl;
                std::cout << "    Value [" << record.value().toString() << "]" << std::endl;
            }
            else
            {
                std::cerr << record.toString() << std::endl;
            }
        }
    }
}
