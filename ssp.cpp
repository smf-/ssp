#include <algorithm>
#include <any>
#include <cstdint>
#include <fstream>
#include <functional>
#include <iostream>
#include <map>

using namespace std;

int main(int argc, char *argv[])
{
	enum class BusType { PrgRoms, TxtRom, ObjRoms, TxtProm, BakProm, ObjProms, TimingProm };
	enum class HighScoreSaveType { no, brazehs, highscoresaves, nvsram };
	enum class ShifterType { no, alu, prom };
	enum class BakRamType { tnx1, tpp1, tpp2 };

	struct Bus
	{
		const BusType type;
		const size_t romSize;
		const vector<string> roms;
	};

	struct Option
	{
		const string defaultParameter;
		const map<string, any> parameters;
	};

	using Encode = function<uint8_t(const uint16_t)>;
	const auto&& bitswap([](auto&& v, auto&&... bits) { vector<int> b{ bits ... }; auto r(v); r = 0; for (size_t i(0); i < b.size(); i++) { r |= ((v >> b[i]) & 1) << (b.size() - i - 1); } return r; });
	vector<uint8_t> prg(0x10000);

	const map<string, Option> options =
	{
		{ "obj-prom-addresses", { "256", {
			{ "256", 256 },
			{ "64", 64 },
		} } },
		{ "prg-encoding", { "tpp2", {
			{ "no", Encode([&](const uint16_t a) { return prg[a]; }) },
			{ "tnx1", Encode([&](const uint16_t a) { return bitswap(prg[bitswap(a ^ 0xfc, 15, 14, 13, 12, 11, 10, 2, 9, 8, 0, 3, 4, 1, 5, 6, 7)], 0, 2, 4, 6, 7, 5, 3, 1); }) },
			{ "tpp2", Encode([&](const uint16_t a) { return bitswap(prg[bitswap(a ^ 0x3f, 15, 14, 13, 12, 11, 10, 5, 9, 8, 7, 4, 3, 6, 2, 1, 0)], 0, 2, 4, 6, 7, 5, 3, 1); }) },
		} } },
		{ "rom-layout", { "tpp2", {
			{ "tnx1", vector<Bus>{
				{ BusType::PrgRoms, 0x1000, { "tnx1-c.2a", "tnx1-c.2b", "tnx1-c.2c", "tnx1-c.2d", "tnx1-c.2e", "tnx1-c.2f", "tnx1-c.2g", "tnx1-c.2h" } },
				{ BusType::TxtRom, 0x800, { "tnx1-v.3h", "" } },
				{ BusType::ObjRoms, 0x1000, { "tnx1-t.1e", "", "tnx1-t.2e", "", "tnx1-t.3e", "", "tnx1-t.5e", "" } },
				{ BusType::TxtProm, 0x20, { "tnx1-t.1a" } },
				{ BusType::BakProm, 0x20, { "tnx1-t.4a" } },
				{ BusType::ObjProms, 0x100, { "tnx1-t.3a", "tnx1-t.2a" } },
				{ BusType::TimingProm, 0x100, { "tnx1-t.3j" } },
			} },
			{ "tpp1", vector<Bus>{
				{ BusType::PrgRoms, 0x1000, { "tpp1-c.2a,2732", "tpp1-c.2b,2732", "tpp1-c.2c,2732", "tpp1-c.2d,2732", "tpp1-c.2e,2732", "tpp1-c.2f,2732", "tpp1-c.2g,2732", "tpp1-c.2h,2732" } },
				{ BusType::TxtRom, 0x800, { "", "tpp1-v.3h,2716" } },
				{ BusType::ObjRoms, 0x2000, { "tpp1-e.1e,2763", "tpp1-e.2e,2763", "tpp1-e.3e,2763", "tpp1-e.5e,2763" } },
				{ BusType::TxtProm, 0x20, { "tpp1-t.1a,82s123" } },
				{ BusType::BakProm, 0x20, { "tpp1-t.4a,82s123" } },
				{ BusType::ObjProms, 0x100, { "tpp1-t.3a,82s129", "tpp1-t.2a,82s129" } },
				{ BusType::TimingProm, 0x100, { "tpp1-t.3j,82s129" } },
			} },
			{ "tpp1bl", vector<Bus>{
				{ BusType::PrgRoms, 0x2000, { "app_exe.3j.2764", "2.3l.2764", "3.3m.2764", "4.3p.2764" } },
				{ BusType::TxtRom, 0x1000, { "5.10gh.2732" } },
				{ BusType::ObjRoms, 0x2000, { "6.6n.2764", "7.6r.2764", "8.6s.2764", "9.6u.2764" } },
				{ BusType::TxtProm, 0x20, { "5.2t.18s030" } },
				{ BusType::BakProm, 0x20, { "6.2u.18s030" } },
				{ BusType::ObjProms, 0x100, { "3.2r.24s10", "4.2s.24s10" } },
				{ BusType::TimingProm, 0x100, { "7.11s.24s10" } },
			} },
			{ "tpp2", vector<Bus>{
				{ BusType::PrgRoms, 0x2000, { "tpp2-c.7a", "tpp2-c.7b", "tpp2-c.7c", "tpp2-c.7e" } },
				{ BusType::TxtRom, 0x1000, { "tpp2-v.5n" } },
				{ BusType::ObjRoms, 0x2000, { "tpp2-v.1e", "tpp2-v.1f", "tpp2-v.1j", "tpp2-v.1k" } },
				{ BusType::TxtProm, 0x20, { "tpp2-c.3a" } },
				{ BusType::BakProm, 0x20, { "tpp2-c.4a" } },
				{ BusType::ObjProms, 0x100, { "tpp2-c.5b", "tpp2-c.5a" } },
				{ BusType::TimingProm, 0x100, { "tpp2-v.7j" } },
			} },
		} } },
		{ "save-prefix", { "ss-", {} } },
	};

	map<string, string> switchParameters;
	for (auto&& switchPair : options)
		switchParameters.insert_or_assign(switchPair.first, switchPair.second.defaultParameter);

	for (int i(1); i < argc; i++)
	{
		string arg(argv[i]);
		if (arg == "--help")
		{
			cout << argv[0] << endl;

			for (auto&& option : options)
			{
				cout << "--" << option.first;

				int index(0);

				for (auto&& parameter : option.second.parameters)
				{
					cout << (index++ ? "|" : " ");
					cout << parameter.first;
				}

				if (!option.second.defaultParameter.empty())
					cout << " (default " << option.second.defaultParameter << ")";

				cout << endl;
			}

			return 0;
		}
		else if (arg.size() >= 2 && arg.compare(0, 2, "--") == 0 && options.count(&arg[2]) != 0)
		{
			if (i >= argc - 1 || argv[i + 1][0] == '-')
				switchParameters.insert_or_assign(&arg[2], "yes");
			else
				switchParameters.insert_or_assign(&arg[2], argv[++i]);
		}
		else if (arg.size() >= 5 && arg.compare(0, 5, "--no-") == 0 && options.count(&arg[5]) != 0)
			switchParameters.insert_or_assign(&arg[5], "no");
		else
		{
			cerr << "invalid parameter " << arg << endl;
			return 1;
		}
	}

	map<string, const any> optionValues;
	for (auto&& switchPair : options)
	{
		auto&& parameter(switchParameters.at(switchPair.first));
		if (switchPair.second.parameters.size() == 0)
			optionValues.emplace(switchPair.first, parameter);
		else if (switchPair.second.parameters.count(parameter) != 0)
		{
			optionValues.emplace(switchPair.first, switchPair.second.parameters.at(parameter));
		}
		else
		{
			cerr << "Invalid parameter value " << switchPair.first << " \"" << parameter << "\"" << endl;
			return 1;
		}
	}

	auto&& objPromAddresses(any_cast<const int>(optionValues.at("obj-prom-addresses")));
	auto&& prgEncoding(any_cast<const Encode>(optionValues.at("prg-encoding")));
	auto&& romLayout(any_cast<const vector<Bus> &>(optionValues.at("rom-layout")));
	auto&& savePrefix(any_cast<const string>(optionValues.at("save-prefix")));

	if (savePrefix == "" && switchParameters.at("rom-layout") == "tnx1")
	{
		cerr << "save-prefix required with rom-layout tnx1" << endl;
		return 1;
	}

	auto&& tnx1RomLayout(any_cast<const vector<Bus> &>(options.at("rom-layout").parameters.at("tnx1")));

	map<BusType, vector<uint8_t>> busBuffers;

	constexpr uint16_t prgStart(0x7000);

	for (auto&& bus : tnx1RomLayout)
	{
		auto&& busBuffer(vector<uint8_t>(bus.romSize * bus.roms.size()));

		size_t offset(0);
		for (auto&& rom : bus.roms)
		{
			if (bus.type == BusType::PrgRoms && offset == prgStart)
				// not populated
				fill_n(&busBuffer[offset], bus.romSize, 0xff);
			else if (!rom.empty())
			{
				cout << "loading " << rom << endl;

				ifstream s(rom, ifstream::binary);
				s.read((char *)&busBuffer[offset], bus.romSize);
				if (s.fail())
				{
					cerr << "load error" << endl;
					return 1;
				}
				/// TODO: hold file open to prevent it being overwritten?
				s.close();
			}
			else if (offset > 0)
				// duplicate previous file
				copy_n(&busBuffer[offset - bus.romSize], bus.romSize, &busBuffer[offset]);

			offset += bus.romSize;
		}

		busBuffers.insert(pair<BusType, vector<uint8_t>>(bus.type, busBuffer));
	}

	auto&& encodedPrg(busBuffers.at(BusType::PrgRoms));
	for (size_t i(0); i < encodedPrg.size(); i++)
		prg[i] = bitswap(encodedPrg[bitswap(i, 15, 14, 13, 12, 11, 10, 8, 7, 0, 1, 2, 4, 5, 9, 3, 6) ^ 0xfc], 3, 4, 2, 5, 1, 6, 0, 7);

	// convert 2 x 4 bit proms to 8 bit colour
	auto&& objProms(busBuffers.at(BusType::ObjProms));
	for (size_t i(0); i < 0x100; i++)
	{
		const auto&& a((i & 0x3f) | ((i & 0x20) << 1));
		objProms[i] = objProms[a] | (objProms[a + 0x100] << 4);
	}
	objProms.resize(0x100);

	for (size_t i(0); i < encodedPrg.size(); i++)
		encodedPrg[i] = prgEncoding(i);

	if (objPromAddresses == 64)
	{
		fill_n(&objProms[0x20], 0x40, 0x00);
		fill_n(&objProms[0x80], 0x80, 0x00);
	}

	// convert from 8 bit colour to 2 x 4 bit proms
	objProms.resize(0x200);
	for (size_t i(0); i < 0x100; i++)
	{
		objProms[i + 0x100] = objProms[i] >> 4;
		objProms[i] &= 0x0f;
	}

	for (auto&& bus : romLayout)
	{
		auto&& buffer(busBuffers.at(bus.type));
		auto romSize(bus.romSize);
		size_t offset(0);

		for (auto&& rom : bus.roms)
		{
			if (!rom.empty() && offset < buffer.size())
			{
				string name = savePrefix + rom;

				cout << "saving " << name << endl;

				ofstream s(name, ofstream::binary);
				s.write((const char *)&buffer[offset], romSize);
				if (s.fail())
				{
					cerr << "save error" << endl;
					return 1;
				}
				s.close();
			}

			offset += romSize;
		}
	}
}
