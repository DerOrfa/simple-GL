#define BOOST_SPIRIT_DEBUG_PRINT_SOME 500
#define BOOST_SPIRIT_DEBUG 1

#include <stdio.h>
#include <stdlib.h>

#include <qt_glue/sglqtspace.h>
#include <text/sgl3dtext.h>
#include <text/sglconsole.h>

#include <QApplication>
#include <boost/fusion/include/std_pair.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>
#include <boost/spirit/include/qi_char_class.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_stl.hpp>
#include <fstream>

typedef std::vector<float> vector;
typedef std::vector<vector> polygon;
typedef std::pair<vector,polygon> facet;

class STLObj : public SGLFlObj{
	std::string m_filename;
	std::list<facet> facets;
	bool parsefile(std::ifstream &f)
	{
		using namespace boost::spirit;
		namespace phoenix = boost::phoenix;
		
		typedef BOOST_TYPEOF( boost::spirit::ascii::space - qi::eol  ) skip_type;
		
		
		istream_iterator first(f), last;
		qi::rule<istream_iterator, vector(),skip_type> vec( qi::repeat(3)[qi::float_], "vector" );
		qi::rule<istream_iterator, polygon(),skip_type> ply("outer loop" >> +("vertex" >> vec) >> "endloop","polygon");
		qi::rule<istream_iterator, facet(),skip_type > fct(lit("facet normal") >> vec >> ply >> "endfacet" );
		qi::rule<istream_iterator,skip_type > solid(
			"solid" >>
			*fct[phoenix::push_back(phoenix::ref(facets),boost::spirit::_1)] >> 
			"endsolid" 
		);
		
	 	BOOST_SPIRIT_DEBUG_NODE(vec);
		BOOST_SPIRIT_DEBUG_NODE(ply);
		BOOST_SPIRIT_DEBUG_NODE(fct);
		BOOST_SPIRIT_DEBUG_NODE(solid);
		
		bool ok = qi::phrase_parse(
			first, last, 
			solid,
			ascii::space - qi::eol
		);
		
		return ok;
		// 	return r;
	}
public:
	STLObj(std::string filename,MaterialPtr Material = MaterialPtr ()):SGLFlObj(Material),m_filename(filename){
		std::ifstream f(filename.c_str());
		f.unsetf(std::ios::skipws);
		f.exceptions ( std::ifstream::failbit | std::ifstream::badbit );
		parsefile(f);
		qDebug("Parsed %d facets from %s",facets.size(),filename.c_str());
	}
    virtual void generate(){
		for(std::list<facet>::const_iterator f=facets.begin();f!=facets.end();f++){
			const vector &normal=f->first;
			const polygon &vertexes=f->second;
			glBegin(GL_POLYGON);
			glNormal3fv(normal.data());
			for(polygon::const_iterator v=vertexes.begin();v!=vertexes.end();v++){
				glVertex3fv(v->data());
			}
			glEnd();
		}
	}
    virtual SGLVektor getCenter() const{return SGLVektor(0,0,0);}
};

int main(int argc, char *argv[])
{
	QApplication a(argc,argv);
	SGLprintState("Initialisiere Schnittstelle ...");
	SGLqtSpace *w= new SGLqtSpace(NULL, "stlview");
	w->show();
	
	for(int i=1;i<argc;i++){
		SGLshPtr< STLObj > obj=SGLshPtr_new<STLObj>(argv[i]);
		obj->Scale(1./30);
		w->registerObj(obj);
	}

	SGLprintState("fertsch");
	a.connect( &a, SIGNAL(lastWindowClosed()), SLOT(quit()) );
	return a.exec();

}
