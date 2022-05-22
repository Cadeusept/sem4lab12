#include <example.hpp>
#include <boost/program_options.hpp>
#include <string>

namespace po = boost::program_options;

int main(int argc, char* argv[]) {
  po::options_description desc("Allowed options");
  desc.add_options()
      ("help", "produce help message")
      ("log_level", po::value<std::string>(), "set log-level "
                                            "<Release|Debug>, Debug by default")
      ("source", po::value<std::string>()->default_value("data.txt"),
               "Name of source file\n")
      ("threshold", po::value<int>()->default_value(1),
       "Enter threshold value\n");
  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify(vm);

  if (vm.count("help")) {
    std::cout << desc << std::endl;
    return 0;
  }

  if (vm.count("log_level") &&
        std::string("Release release RELEASE").find(
            vm["log_level"].as<std::string>()) != std::string::npos)
      Log::GetInstance().Setting(false);
  else
      Log::GetInstance().Setting(true);
  UsedMemory used_memory;

  PageContainer page{};
  std::ifstream in(vm["source"].as<std::string>());

  page.RawLoad(in);
  page.DataLoad(vm["threshold"].as<int>());

  Log::GetInstance().Write("Used memory: " +
                             std::to_string(used_memory.Used()));
  Log::GetInstance().
        Write("Number skips in this lap: " +
              std::to_string(Histogram::GetInstance().GetNumSkip()));
  Log::GetInstance().
        Write("Average score: " +
              std::to_string(Histogram::GetInstance().GetAvg()));

  page.PrintTable();

  page.DataLoad(vm["threshold"].as<int>()+3);
  Log::GetInstance().Write("Used memory: " +
                             std::to_string(used_memory.Used()));
  Log::GetInstance().
        Write("Number skips in this lap: " +
              std::to_string(Histogram::GetInstance().GetNumSkip()));
  Log::GetInstance().
        Write("Average score: " +
              std::to_string(Histogram::GetInstance().GetAvg()));

  //  throw po::error("Bad syntax, check options at --help");
  return 0;
}