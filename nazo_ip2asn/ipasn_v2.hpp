#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <optional>
#include <boost/asio/ip/address.hpp>
#include <boost/algorithm/string.hpp>

namespace Address
{
    using namespace boost::asio::ip;
    using namespace boost::algorithm;
    using namespace std;
    typedef pair<std::string, std::string> DataPair;

    struct IPASN
    {
        /// ip头
        address ip;
        /// asn号(可空)
        std::string asn;
        /// AS_description
        std::string as_description;

        // bool operator<(const IPASN &other) const
        // {
        //     return ip < other.ip;
        // }
    };

    // vector<address> ipv4;
    // vector<address> ipv6;
    // vector<DataPair> ipv4data;
    // vector<DataPair> ipv6data;
    vector<IPASN> ipv4_data;
    vector<IPASN> ipv6_data;

    inline size_t count_lines(const std::string &filename)
    {
        ifstream file(filename);
        return count(istreambuf_iterator<char>(file), istreambuf_iterator<char>(), '\n');
    }

    void read_file(const std::string &filename, vector<IPASN> &data)
    {
        ifstream _file(filename);
        std::string line;

        size_t num_lines = count_lines(filename);
        // ip.reserve(num_lines);
        data.reserve(num_lines);
        while (getline(_file, line))
        {
            vector<std::string> row;
            split(row, line, is_any_of("\t"));

            data.push_back(IPASN{make_address(row[0]), row[2], row[4]});

            // ip.push_back(make_address(row[0]));
            // data.emplace_back(row[2], row[3]);

            if (_file.peek() == EOF)
            {
                data.push_back(IPASN{make_address(row[1]), "NULL", "NULL"});
                // ip.push_back(make_address(row[1]));
                break;
            }
        }
        _file.close();
    }

    void init(const std::string &ipv4file, const std::string &ipv6file)
    {
        read_file(ipv4file, ipv4_data);
        read_file(ipv6file, ipv6_data);
        if (ipv4_data.empty() || ipv6_data.empty())
        {
            cout << "\033[31m"
                 << "Error: ipasn.hpp: init(): ipv4 or ipv6 file is empty, Please go to https://iptoasn.com/ and download the latest files."
                 << endl
                 << "ipv4 file: Please download the latest version file from https://iptoasn.com/data/ip2asn-v4.tsv.gz and unzip it."
                 << endl
                 << "ipv6 file: Please download the latest version file from https://iptoasn.com/data/ip2asn-v6.tsv.gz and unzip it."
                 << "\033[0m" << endl;
        }
        else
        {
            cout << "\033[32m\033[1m"
                 << "ipasn.hpp: init(): ipv4 and ipv6 file loaded" << endl;
            cout << "ipasn.hpp: init(): ipv4 size: "
                 << "\033[4m" << ipv4_data.size() << "\033[0m" << endl;
            cout << "\033[32m\033[1m"
                 << "ipasn.hpp: init(): ipv6 size: "
                 << "\033[4m" << ipv6_data.size() << "\033[0m" << endl;
        }
    }

    inline std::optional<IPASN> binary_search(vector<IPASN> &data, const address &ip)
    {
        auto it = lower_bound(data.begin(), data.end(), ip, [](const IPASN &lhs, const address &rhs)
                              { return lhs.ip < rhs; });
        if (it == data.begin() || it == data.end())
        {
            return std::nullopt;
        }
        return *(it - 1);
        // return distance(data.begin(), it) - 1;
    }

    DataPair lookup(const std::string &ip)
    {
        address _ip = make_address(ip);
        bool is_ipv4 = _ip.is_v4();

        auto &&target_data = is_ipv4 ? ipv4_data : ipv6_data;

        if (auto ipasn_data = binary_search(target_data, _ip))
        {
            return DataPair{ipasn_data->asn, ipasn_data->as_description};
        }

        throw std::runtime_error("IP not found");
    }

}