#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graphml.hpp>
#include <boost/property_tree/xml_parser.hpp>


struct MyVertex
{
    int id;
};

struct MyEdge
{
    int id;
};

template<typename T_Graph>
void
addToPropertyTree(
    boost::property_tree::ptree & pt,
    std::string path,
    T_Graph & g,
    boost::dynamic_properties & dp
)
{
    //convert graph to property_tree
    boost::property_tree::ptree pt_graph;
    std::stringstream ss;
    boost::write_graphml(ss, g, dp);

    // trim_whitespace is a workaround to avoid printing  &#10;  at each newline
    // http://stackoverflow.com/questions/6572550/boostproperty-tree-xml-pretty-printing
    // https://svn.boost.org/trac/boost/ticket/11600
    boost::property_tree::read_xml(ss, pt_graph, boost::property_tree::xml_parser::trim_whitespace);

    // add graph property tree to global pt
    pt.add_child(path, pt_graph);
}

template<typename T_Graph>
void
getFromPropertyTree(
    boost::property_tree::ptree & pt,
    std::string path,
    T_Graph & g,
    boost::dynamic_properties & dp
)
{
    //extract graph as property tree
    boost::property_tree::ptree recovered_child = pt.get_child(path);

    // write extracted graph as XML for boost bgl
    std::stringstream ss;
    boost::property_tree::xml_writer_settings<std::string> settings(' ', 4);
    boost::property_tree::write_xml(ss, recovered_child, settings);

    // read recovered XML as a boost BGL graph
    boost::read_graphml(ss, g, dp);
}

void
savePropertyTreeAsXML(
    boost::property_tree::ptree & pt,
    std::string path
)
{
    std::ofstream fs;
    fs.open(path);
    boost::property_tree::xml_writer_settings<std::string> settings(' ', 4);
    boost::property_tree::write_xml(fs, pt, settings);
    fs.close();
}

void
readPropertyTreeFromXML(
    boost::property_tree::ptree & pt,
    std::string path
)
{
    std::ifstream fs(path);
    read_xml(fs, pt, boost::property_tree::xml_parser::trim_whitespace);
    fs.close();
}

int main()
{
    using namespace boost;

    adjacency_list<vecS, vecS, directedS, MyVertex, MyEdge> g;

    auto v0 = add_vertex({0}, g);
    auto v1 = add_vertex({1}, g);
    auto v2 = add_vertex({2}, g);
    auto v3 = add_vertex({3}, g);

    add_edge(v0,v1,g);
    add_edge(v1,v2,g);
    add_edge(v2,v3,g);
    add_edge(v3,v0,g);


    //convert graph to property_tree
    property_tree::ptree pt_graph;
    std::stringstream ss;
    dynamic_properties dp1;
    dp1.property("hid", boost::get(&MyVertex::id, g));
    write_graphml(ss, g, dp1);

    // trim_whitespace is a workaround to avoid printing  &#10;  at each newline
    // http://stackoverflow.com/questions/6572550/boostproperty-tree-xml-pretty-printing
    // https://svn.boost.org/trac/boost/ticket/11600
    read_xml(ss, pt_graph, property_tree::xml_parser::trim_whitespace);

    // add graph property tree to global pt
    property_tree::ptree pt_complete;
    pt_complete.add_child("graphs.circular", pt_graph);

    // write XML of global PT
    std::ofstream fs;
    fs.open("/tmp/output.xml");
    property_tree::xml_writer_settings<std::string> settings(' ', 4);
    write_xml(fs, pt_complete, settings);
    fs.close();

    //read global PT as XML from file
    std::ifstream fs2("/tmp/output.xml");
    property_tree::ptree fromXML;
    read_xml(fs2, fromXML, property_tree::xml_parser::trim_whitespace);
    fs2.close();

    //extract graph as property tree
    property_tree::ptree recovered_child = fromXML.get_child("graphs.circular");

    // write extracted graph as XML for boost bgl
    std::stringstream ss2;
    write_xml(ss2, recovered_child, settings);

    // read recovered XML as a boost BGL graph
    adjacency_list<vecS, vecS, directedS, MyVertex, MyEdge> g2;
    dynamic_properties dp2;
    dp2.property("hid", boost::get(&MyVertex::id, g2));
    read_graphml(ss2, g2, dp2);
    std::cout << ss2.str() << std::endl;

    return 0;
}
