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
    write_graphml(ss, g, dynamic_properties());

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
    dynamic_properties dp;
    read_graphml(ss2, g2, dp);
    std::cout << ss2.str() << std::endl;

    return 0;
}
