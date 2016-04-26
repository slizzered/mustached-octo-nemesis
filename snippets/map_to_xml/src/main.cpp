#include <map>
#include <iostream>
#include <boost/property_tree/xml_parser.hpp>


template<typename T1, typename T2>
void mapToPtree(
    boost::property_tree::ptree & pt,
    std::string name,
    std::map<T1,T2> mymap
)
{
    pt.put(name + ".<xmlattr>.size", mymap.size());
    for(auto& i : mymap)
    {
        boost::property_tree::ptree internPt;
        internPt.put("<xmlattr>.first", i.first);
        internPt.put("<xmlattr>.second", i.second);
        pt.add_child(name + ".item", internPt);
    }
}

template<typename T1, typename T2>
void
mapFromPtree(
    boost::property_tree::ptree & pt,
    std::string path,
    std::map<T1,T2>& mymap

)
{
    auto& child = pt.get_child(path);
    for(auto i = child.find("item") ; child.to_iterator(i) != child.end() ; ++i)
    {
        auto& item = i->second;
        auto first = item.get< T1 >( "<xmlattr>.first" );
        auto second = item.get< T2 >( "<xmlattr>.second" );
        mymap[first] = second;
    }
}


int main()
{
    boost::property_tree::ptree pt;
    std::map<int, int> mymap;
    boost::property_tree::xml_writer_settings<std::string> settings(' ', 4);
    std::stringstream ss;

    mymap[0]=13;
    mymap[1]=14;

    // convert Map to PTree and write the resulting XML
    mapToPtree(pt, "SW.MYMAP", mymap);
    boost::property_tree::write_xml(ss, pt, settings);
    std::cout << ss.str() << std::endl;

    // Convert Ptree back to map
    mymap.clear();
    mapFromPtree(pt, "SW.MYMAP", mymap);

    // Again, convert mal to a PTree and compare the resulting XML to the first
    pt.clear();
    std::stringstream().swap(ss); // clear stringstream
    mapToPtree(pt, "SW.MYMAP", mymap);
    boost::property_tree::write_xml(ss, pt, settings);
    std::cout << ss.str() << std::endl;

    return 0;
}
