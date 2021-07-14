#include <iostream>
#include <cctype>
#include <cstring>
extern "C" {
#include <unistd.h>
}
#include <IoSys/MhFilesystem.hpp>
#include <Io/MhFile.hpp>
#include <Media/Image/MhStandardColourFormat.hpp>
#include <Media/Image/MhPNG.hpp>
#include <Media/Image/MhGIF.hpp>
#include <map>
#include <vector>
#include <cstring>
#include <cmath>

using Pixel = MH33::GFX::RGB<uint8_t>;
using Pixel2 = MH33::GFX::RGBA<uint8_t>;

static const std::map<std::string,std::string> paths = {
	{"micronesid", "pheno_regions/micronesid.png"},
	{"south_polynesid", "pheno_regions/southpolynesid.png"},
	{"fijid", "pheno_regions/fijid.png"},
	{"basic_patagonid", "pheno_regions/patagonid.png"},
	{"basic_andid", "pheno_regions/andid.png"},
	{"basic_amazonid", "pheno_regions/amazonid.png"},
	{"basic_margid", "pheno_regions/margid.png"},
	{"basic_centralid", "pheno_regions/centralid.png"},
	{"basic_silvid", "pheno_regions/silvid.png"},
	{"basic_eskimid", "pheno_regions/eskimid.png"},
	{"basic_pacifid", "pheno_regions/pacifid.png"},
	{"basic_khoid", "pheno_regions/khoid.png"},
	{"basic_sanid", "pheno_regions/sanid.png"},
	{"basic_bambutid", "pheno_regions/bambutid.png"},
	{"basic_bantuid", "pheno_regions/bantuid.png"},
	{"basic_australid", "pheno_regions/australid.png"},
	{"basic_negritid", "pheno_regions/negritid.png"},
	{"basic_melanesid", "pheno_regions/melanesid.png"},
	{"brunn", "pheno_regions/brunn.png" },
	{"honshu", "pheno_regions/honshu.png"},
	{"okinawa", "pheno_regions/okinawa.png"},
	{"toalid", "pheno_regions/toalid.png"},
	{"senoid", "pheno_regions/senoid.png"},
	{"robust_polynesid", "pheno_regions/robust_polynesid.png"},
	{"nesiotid", "pheno_regions/nesiotid.png"},
	{"dayakid", "pheno_regions/dayakid.png"},
	{"keltic_nordid", "pheno_regions/keltic_nordid.png"},
	{"subnordid", "pheno_regions/subnordid.png"},
	{"atlanto_mediterranean", "pheno_regions/atlanto_mediterranean.png"},
	{"canarid", "pheno_regions/canarid.png"},
	{"transmediterranid", "pheno_regions/transmediterranid.png"},
	{"gracilemediterranid", "pheno_regions/gracilemediterranid.png"},
	{"dinarid", "pheno_regions/dinarid.png"},
	{"armenoid", "pheno_regions/armenoid.png"},
	{"aoshima", "pheno_regions/aoshima.png"},
	{"yakonin", "pheno_regions/yakonin.png"},
	{"west_alpinid", "pheno_regions/west_alpinid.png"},
	{"troender", "pheno_regions/tronder.png"},
	{"tonkinesid", "pheno_regions/tonkinesid.png"},
	{"tibetid", "pheno_regions/tibetid.png"},
	{"tavastid", "pheno_regions/tavastid.png"},
	{"targid", "pheno_regions/targid.png"},
	{"strandid", "pheno_regions/strandid.png"},
	{"shanid", "pheno_regions/shanid.png"},
	{"savolaxid", "pheno_regions/savolaxid.png"},
	{"satsuma", "pheno_regions/satsuma.png"},
	{"pre_slavic", "pheno_regions/pre-slavic.png"},
	{"pontid", "pheno_regions/pontid.png"},
	{"paleo_sardinian", "pheno_regions/paleo_sardinian.png"},
	{"paleo_atlantid", "pheno_regions/paleo_atlantid.png"},
	{"north_pontid", "pheno_regions/north_pontid.png"},
	{"north_indid", "pheno_regions/north_indid.png"},
	{"north_atlantid", "pheno_regions/north_atlantid.png"},
	{"norid", "pheno_regions/norid.png"},
	{"neo_danubian", "pheno_regions/neo_danubian.png"},
	{"mtebid", "pheno_regions/mtebid.png"},
	{"mountain_indid", "pheno_regions/mountain_indid.png"},
	{"moorish", "pheno_regions/moorish.png"},
	{"manchukorean", "pheno_regions/manchukorean.png"},
	{"litorid", "pheno_regions/litorid.png"},
	{"libyid", "pheno_regions/libyid.png"},
	{"ladogan", "pheno_regions/ladogan.png"},
	{"kham", "pheno_regions/kham.png"},
	{"ishikawa", "pheno_regions/ishikawa.png"},
	{"iranid", "pheno_regions/iranid.png"},
	{"indo_nordid", "pheno_regions/indo_nordid.png"},
	{"indo_iranid", "pheno_regions/indo_iranid.png"},
	{"indo_brachid", "pheno_regions/indo_brachid.png"},
	{"huanghoid", "pheno_regions/huanghoid.png"},
	{"halstatt", "pheno_regions/halstatt.png"},
	{"sinhalesid", "pheno_regions/sinhalesid.png"},
	{"siwa", "pheno_regions/siwa.png"},
	{"gracile_indid", "pheno_regions/gracileindid.png"},
	{"gorid", "pheno_regions/gorid.png"},
	{"gobid", "pheno_regions/gobid.png"},
	{"fenno_nordid", "pheno_regions/fenno_nordid.png"},
	{"faelid", "pheno_regions/faelid.png"},
	{"eurafricanid", "pheno_regions/eurafricanid.png"},
	{"egyptid", "pheno_regions/egyptid.png"},
	{"east_brachid", "pheno_regions/east_brachid.png"},
	{"choshiu", "pheno_regions/choshiu.png"},
	{"chikuzen", "pheno_regions/chikuzen.png"},
	{"chukiangid", "pheno_regions/chukiangid.png"},
	{"changkiangid", "pheno_regions/changkiangid.png"},
	{"central_pamirid", "pheno_regions/central_pamirid.png"},
	{"central_brachid", "pheno_regions/central_brachid.png"},
	{"carpathid", "pheno_regions/carpathid.png"},
	{"borreby", "pheno_regions/borreby.png"},
	{"berberid", "pheno_regions/berberid.png"},
	{"baskid", "pheno_regions/baskid.png"},
	{"asian_alpine", "pheno_regions/asian_alpine.png"},
	{"assyroid", "pheno_regions/assyroid.png"},
	{"plains_pamirid", "pheno_regions/plains_pamirid.png"},
	{"aralid", "pheno_regions/aralid.png"},
	{"arabid", "pheno_regions/arabid.png"},
	{"annamid", "pheno_regions/annamid.png"},
	{"anglo_saxon", "pheno_regions/anglo_saxon.png"},
	{"andronovo_turanid", "pheno_regions/andronovo-turanid.png"},
	{"anatolid", "pheno_regions/anatolid.png"},
	{"alfoeld", "pheno_regions/alfoeld.png"},
	{"aisto_nordid", "pheno_regions/aisto_nordid.png"},
	{"african_alpine", "pheno_regions/african_alpine.png"},
	{"baykal", "pheno_regions/baykal.png"},
	{"yemenid", "pheno_regions/yemenid.png"},
	{"samoyedic", "pheno_regions/samoyedic.png"},
	{"uralid", "pheno_regions/uralid.png"},
	{"volgid", "pheno_regions/volgid.png"},
	{"north_lappid", "pheno_regions/northlappid.png"},
	{"scando_lappid", "pheno_regions/scandolappid.png"},
	{"north_lappid", "pheno_regions/northlappid.png"},
	{"proto_iranid", "pheno_regions/protoiranid.png"},
	{"east_ethiopid", "pheno_regions/eastethiopid.png"},
	{ "east_pamirid", "pheno_regions/east_pamirid.png" },
	{"transcaspian", "pheno_regions/transcaspian.png"},
	{"keralid", "pheno_regions/keralid.png"},
	{"malabarese", "pheno_regions/malabarese.png"},
	{"kolid", "pheno_regions/kolid.png"},
	{"karnatid", "pheno_regions/karnatid.png"},
	{"vedda", "pheno_regions/vedda.png"},
	{"south_gondid", "pheno_regions/southgondid.png"},
	{"north_gondid", "pheno_regions/northgondid.png"},
	{"arabian_veddoid", "pheno_regions/arabianveddoid.png"},
	{"proto_malayid", "pheno_regions/protomalayid.png"},
	{"khmerid", "pheno_regions/khmerid.png"},
	{"east_palaungid", "pheno_regions/eastpalaungid.png"},
	{"east_shanid", "pheno_regions/eastshanid.png"},
	{"south_palaungid", "pheno_regions/southpalaungid.png"},
	{"kachinid", "pheno_regions/kachinid.png"},
	{"deutero_malayid", "pheno_regions/deuteromalayid.png"},
	{"palaungid", "pheno_regions/palaungid.png"},
	{"shari", "pheno_regions/shari.png"},
	{"guineo_camerunian", "pheno_regions/guineocamerunian.png"},
	{"guinesid", "pheno_regions/guinesid.png"},
	{"bobo", "pheno_regions/bobo.png"},
	{"casamance", "pheno_regions/casamance.png"},
	{"senegalid", "pheno_regions/senegalid.png"},
	{"dinkaid", "pheno_regions/dinkaid.png"},
	{"paleo_saharid", "pheno_regions/paleosaharid.png"},
	{"middle_nile", "pheno_regions/middlenile.png"},
	{"proto_nordid", "pheno_regions/protonordid.png"},
	{"fezzanid", "pheno_regions/fezzanid.png"},
	{"omotic", "pheno_regions/omotic.png"},
	{"proto_ethiopid", "pheno_regions/protoethiopid.png"},
	{"saharan_ethiopid", "pheno_regions/saharanethiopid.png"},
	{"central_ethiopid", "pheno_regions/centralethiopid.png"},
	{"north_ethiopid", "pheno_regions/northethiopid.png"},
	{"west_ethiopid", "pheno_regions/westethiopid.png"},
	{"equatorialsudanid", "pheno_regions/equatorialsudanid.png"},
	{"westcongolesid", "pheno_regions/westcongolesid.png"},
	{"congolesid", "pheno_regions/congolesid.png"},
	{"sudanid", "pheno_regions/sudanid.png"}
};

static constexpr uint8_t charToHex(char upper, char lower) {
	uint8_t num = 0;
	switch (upper) {
	case '0': num |= 0x00; break;
	case '1': num |= 0x10; break;
	case '2': num |= 0x20; break;
	case '3': num |= 0x30; break;
	case '4': num |= 0x40; break;
	case '5': num |= 0x50; break;
	case '6': num |= 0x60; break;
	case '7': num |= 0x70; break;
	case '8': num |= 0x80; break;
	case '9': num |= 0x90; break;
	case 'a': num |= 0xA0; break;
	case 'b': num |= 0xB0; break;
	case 'c': num |= 0xC0; break;
	case 'd': num |= 0xD0; break;
	case 'e': num |= 0xE0; break;
	case 'f': num |= 0xF0; break;
	case 'A': num |= 0xA0; break;
	case 'B': num |= 0xB0; break;
	case 'C': num |= 0xC0; break;
	case 'D': num |= 0xD0; break;
	case 'E': num |= 0xE0; break;
	case 'F': num |= 0xF0; break;
	default: break;
	}
	switch (lower) {
	case '0': num |= 0x00; break;
	case '1': num |= 0x01; break;
	case '2': num |= 0x02; break;
	case '3': num |= 0x03; break;
	case '4': num |= 0x04; break;
	case '5': num |= 0x05; break;
	case '6': num |= 0x06; break;
	case '7': num |= 0x07; break;
	case '8': num |= 0x08; break;
	case '9': num |= 0x09; break;
	case 'a': num |= 0x0A; break;
	case 'b': num |= 0x0B; break;
	case 'c': num |= 0x0C; break;
	case 'd': num |= 0x0D; break;
	case 'e': num |= 0x0E; break;
	case 'f': num |= 0x0F; break;
	case 'A': num |= 0x0A; break;
	case 'B': num |= 0x0B; break;
	case 'C': num |= 0x0C; break;
	case 'D': num |= 0x0D; break;
	case 'E': num |= 0x0E; break;
	case 'F': num |= 0x0F; break;
	default: break;
	}
	return num;
}

void doTheComparisons(const std::vector<Pixel>& mask, Pixel colour);
double compareWith(const std::vector<Pixel>& mask, const std::vector<Pixel>& pheno, Pixel colour);
double compareWith(const std::vector<Pixel>& mask, const MH33::GFX::DecodeTarget& pheno, Pixel colour);
bool cmp(std::pair<std::string, int>& a, std::pair<std::string, int>& b);

int main(int argc, char **argv)
{
	char *mvalue = nullptr;
	char *cvalue = nullptr;
	int c;
	while ((c = getopt (argc, argv, "hm:c:")) != -1) {
		switch (c) {
		case 'h':
			std::cout << "Usage: " << argv[0] << " -m <culturefile.png> -c <colournameHTML>" << std::endl;
			break;
		case 'm':
			mvalue = optarg;
			break;
		case 'c':
			cvalue = optarg;
			break;
		}
	}
	if(mvalue && cvalue) {
		if(strlen(cvalue) < 6) {
			std::cout << "Cvalue is not a proper colour!" << std::endl;
			return 0;
		}
		Pixel rgb;
		rgb.r = charToHex(cvalue[0],cvalue[1]);
		rgb.g = charToHex(cvalue[2],cvalue[3]);
		rgb.b = charToHex(cvalue[4],cvalue[5]);
		MH33::Io::File file(mvalue, MH33::Io::Mode::READ);
		MH33::GFX::DecodeTarget target;
		MH33::GFX::PNG::decode(file,target);
		std::vector<Pixel> maskPixels(target.frames[0].width * target.frames[0].height);
		std::memcpy(maskPixels.data(),target.frames[0].imageData.data(),target.frames[0].imageData.size());
		doTheComparisons(maskPixels,rgb);
	} else std::cout << "Usage: " << argv[0] << " -m <culturefile.png> -c <colournameHTML>" << std::endl;
	return 0;
}
inline bool ends_with(std::string const & value, std::string const & ending)
{
	if (ending.size() > value.size()) return false;
	return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
}

bool cmp(std::pair<std::string, int>& a, std::pair<std::string, int>& b) {
	return a.second > b.second;
}

void doTheComparisons(const std::vector<Pixel>& mask, Pixel colour) {
	std::map<std::string,double> phenoOccurences;
	for(auto it = std::begin(paths); it != std::end(paths); ++it) {
		MH33::Io::File file(it->second, MH33::Io::Mode::READ);
		MH33::GFX::DecodeTarget target;
		if(ends_with(it->second,".png")) MH33::GFX::PNG::decode(file,target);
		else if(ends_with(it->second,".PNG")) MH33::GFX::PNG::decode(file,target);
		else if(ends_with(it->second,".gif")) MH33::GFX::GIF::decode(file,target);
		else if(ends_with(it->second,".GIF")) MH33::GFX::GIF::decode(file,target);
		phenoOccurences[it->first] = compareWith(mask,target,colour);
	}
	double summa = 0.0;
	for(auto it = std::begin(phenoOccurences); it != std::end(phenoOccurences); ++it) {
		summa += it->second;
	}
	summa = 1.0/summa;
	std::vector<std::pair<std::string,int>> intMap;
	for(auto it = std::begin(phenoOccurences); it != std::end(phenoOccurences); ++it) {
		if(it->second >= 0.1) intMap.push_back(std::make_pair(it->first,int(std::ceil((it->second*summa)*100.0))) );
	}
	sort(intMap.begin(), intMap.end(), cmp);
	for(const auto& it : intMap) {
		std::cout << "\t\t\t" << it.second << " = " << it.first << "\n";
	}
}

double compareWith(const std::vector<Pixel>& mask, const MH33::GFX::DecodeTarget& pheno, Pixel colour) {
	if(!pheno.frames.size()) return 0.0;
	std::vector<Pixel> phenoPixels;
	if(pheno.format == MH33::GFX::Format::RGB8U) {
		phenoPixels.resize(pheno.frames[0].width * pheno.frames[0].height);
		std::memcpy(phenoPixels.data(),pheno.frames[0].imageData.data(),pheno.frames[0].imageData.size());
	} else if(pheno.format == MH33::GFX::Format::RGBA8U) {
		std::vector<Pixel2> phenoPixels2(pheno.frames[0].width * pheno.frames[0].height);
		std::memcpy(phenoPixels2.data(),pheno.frames[0].imageData.data(),pheno.frames[0].imageData.size());
		phenoPixels.resize(pheno.frames[0].width * pheno.frames[0].height);
		for(size_t i = 0; i < phenoPixels2.size(); ++i) {
			phenoPixels[i].r = phenoPixels2[i].r;
			phenoPixels[i].g = phenoPixels2[i].g;
			phenoPixels[i].b = phenoPixels2[i].b;
		}
	} else if(pheno.format == MH33::GFX::Format::INDEXED) {
		phenoPixels.resize(pheno.frames[0].width * pheno.frames[0].height);
		const Pixel* palette = reinterpret_cast<const Pixel*>(pheno.palette->palette.data());
		const uint8_t* indices = reinterpret_cast<const uint8_t*>(pheno.frames[0].imageData.data());
		for(size_t i = 0; i < pheno.frames[0].imageData.size();++i) {
			phenoPixels[i] = palette[indices[i]];
		}
	}
	return compareWith(mask,phenoPixels,colour);
}
double compareWith(const std::vector<Pixel> &mask, const std::vector<Pixel> &pheno, Pixel colour) {
	if(!mask.size() || !pheno.size()) return 0.0;
	if(mask.size() != pheno.size()) return 0.0;
	double toReturn = 0.0;
	for(size_t i = 0; i < mask.size(); ++i) {
		if( (mask[i].r == colour.r) && (mask[i].g == colour.g) && (mask[i].b == colour.b) ) {
			if( (pheno[i].r == 255) && (pheno[i].g == 255) ) toReturn += 1.0;
			else if( (pheno[i].r == 127) && (pheno[i].g == 127) ) toReturn += 0.05;
		}
	}
	return toReturn;
}
