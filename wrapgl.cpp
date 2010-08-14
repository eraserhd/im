#include <algorithm>
#include <boost/foreach.hpp>
#include <boost/tuple/tuple.hpp>
#include <cassert>
#include <cctype>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>
using namespace std;

struct api {
    std::string return_type;
    std::string name;
    std::vector<std::pair<std::string, std::string> > args;
};

std::string trim(std::string l) {
    while (!l.empty() && isspace(l[0]))
        l = l.substr(1);
    while (!l.empty() && isspace(l[l.size()-1]))
        l = l.substr(0,l.size()-1);
    return l;
}

std::pair<std::string, std::string> break_type_and_name(std::string const& s) {
    string::size_type beginning_of_id = s.size();
    while (beginning_of_id > 0 && iscsym(s[beginning_of_id-1]))
        --beginning_of_id;

    const std::string name = s.substr(beginning_of_id);
    const std::string return_type = trim(s.substr(0,beginning_of_id));
    return make_pair(return_type, name);
}

std::vector<api> load_apis() {
    ifstream in("opengl_apis.txt",ios::in);
    string l;
    vector<api> result;
    while (getline(in,l)) {
        l = trim(l);
        if (l.empty())
            continue;


        const string::size_type lparen = l.find('(',0);
        assert(string::npos != lparen);

        const string::size_type rparen = l.find_last_of(")");
        assert(string::npos != rparen);

        api a;
        boost::tie(a.return_type, a.name) = break_type_and_name(trim(l.substr(0, lparen)));

        string::size_type i = lparen+1;
        while (i < rparen) {
            string::size_type j = l.find(',',i);
            if (string::npos == j)
                j = rparen;

            a.args.push_back(break_type_and_name(trim(l.substr(i,j-i))));
            i = j+1;
        }

        result.push_back(a);
    }
    in.close();
    return result;
}


void real_body_generator(api const& a, ostream& o) {
    o << "::" << a.name << "(";
    bool first = true;
    pair<string, string> arg;
    BOOST_FOREACH(arg, a.args) {
        if (!first)
            o << ", ";
        first = false;
        o << arg.second;
    }
    o << ");";
}

void mock_body_generator(api const& a, ostream& o) {
    string struct_name = "C_" + a.name;
    o << "boost::shared_ptr<" << struct_name << "> p(new " << struct_name << "(";
    bool first = true;
    pair<string,string> arg;
    BOOST_FOREACH(arg, a.args) {
        if (!first)
            o << ", ";
        first = false;
        o << arg.second;
    }
    o << ")); ";
    o << "calls.push_back(p);";
}

void inline_static_wrapper(api const& a, ostream& o, void (* body_generator) (api const&, ostream&)) {
    o << "inline static " << a.return_type << " " << a.name << "(";
    bool first = true;
    pair<string,string> arg;
    BOOST_FOREACH(arg, a.args) {
        if (!first)
            o << ", ";
        first = false;
        o << arg.first << " " << arg.second;
    }
    o << ") { ";

    body_generator(a,o);

    o << " }";
}

void make_real_glwrap_hpp(std::vector<api> const& apis) {
    ofstream out("real_glwrap.hpp", ios::out);
    BOOST_FOREACH(api a, apis) {
        inline_static_wrapper(a,out,real_body_generator);
        out << endl;
    }
    out.close();
}

string call_wrapper_struct(api const& a) {
    ostringstream o;
    o << "struct C_" << a.name << " : public C_base {" << endl;
    o << "  C_" << a.name << "(";
    pair<string,string> arg;
    bool first = true;
    BOOST_FOREACH(arg, a.args) {
        if (!first)
            o << ", ";
        first = false;
        o << arg.first << " " << arg.second;
    }
    o << ")" << endl;
    o << "    ";
    first = true;
    BOOST_FOREACH(arg, a.args) {
        if (first)
            o << ": ";
        else
            o << ", ";
        first = false;
        o << arg.second << "(" << arg.second << ")";
    }
    o << endl;
    o << "  {}" << endl;
    first = true;
    BOOST_FOREACH(arg, a.args) {
        o << "  " << arg.first << " " << arg.second << ";" << endl;
    }
    o << "};" << endl;
    return o.str();
}

void make_mock_glwrap_hpp(std::vector<api> const& apis) {
    ofstream out("mock_glwrap.hpp", ios::out);
    BOOST_FOREACH(api a, apis) {
        out << call_wrapper_struct(a) << endl;
        inline_static_wrapper(a,out,mock_body_generator);
        out << endl;
    }
    out.close();
}

int main() {
    std::vector<api> apis = load_apis();
    make_real_glwrap_hpp(apis);
    make_mock_glwrap_hpp(apis);
    return 0;
}

