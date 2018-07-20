#include "text.h"

#include <cstring>

#include "../../render/render.h"
#include "../../debug/log.h"

using namespace Engine::Graphics::UI;
using namespace Engine::Math;

// Konstruktory
Text::Text():
	upd(true), w(0), h(0), rw(0), rh(0), line(0), space(0), tab(0), alpha(1.0f), str(), opts(DEFAULT), font(), /*cam(),*/ buff()
	{
	//
	}

Text::Text(const std::string& font, const std::string& str, int w, int h, int line, int space, int tab):
	upd(true), w(w), h(h), rw(0), rh(0), line(line), space(space), tab(tab), alpha(1.0f), str(), opts(DEFAULT), font(), /*cam(),*/ buff()
	{
	setFont(font);
	setStr(str);

	init();
	}

//Text::Text(const Text& txt)
//	{
//	//
//	}


bool Text::init(const std::string& font, const std::string& str, int w, int h, int line, int space, int tab)
	{
	this->w=w;
	this->h=h;

	this->line=line;
	this->space=space;
	this->tab=tab;

	setFont(font);
	setStr(str);

	if(!this->font)
		return false;

	return init();
	}

bool Text::init()
	{
	if(!buff.init(w, h, Render::FrameBuffer::FBO_TEXTURE))
		return false;

	/*cam=Camera(Camera::GUI, w, h, -1, 1);
	//cam=Orientation::GUI;
	cam.setPos(Vector(w/2, h/2, 0));*/

	return true;
	}

void Text::update()
	{
	if(!upd)
		return;
	upd=false;

	parse();
	print();
	}

void Text::print(const Orientation& orientation) const
	{
	Render::getInstance().draw(orientation, buff);
	}

void Text::clear()
	{
	upd=true;
	buff.clear();
	font=nullptr;
	}


void Text::setStr(const std::string& str)
	{
	if(str==this->str)
		return;

	upd=true;

	this->str=str;
	}

void Text::addStr(const std::string& astr)
	{
	setStr(str+astr);
	}

void Text::setFont(const std::string& name)
	{
	font=SpritePtr(name);

	if(!font)
		return;

	if(line<0)
		line=font->getAnimation(0u).getDirection(Sprite::DirectionName::DEFAULT).getFrame(0u).getHeight();
	if(space<0)
		space=font->getAnimation(0u).getDirection(Sprite::DirectionName::DEFAULT).getFrame(0u).getWidth();
	if(tab<0)
		tab=space*4;
	}


void Text::parse()
	{
	// Na poczatek - skasuj poprzednie informacje
	lines.clear();

	LineInfo li;
	li.start=0;
	li.length=0;
	li.pixs=0;

// Przelot pierwszy, lamanie po enterach, dlugosc linii w znakach
	for(unsigned i=0u; i<str.size(); ++i)
		{
		char c=str[i];
	// Nowa linia
		if((c=='\r' && i+1<str.size() && str[i+1]=='\n') || c=='\n')
			{
			lines.push_back(li);
			li.start=i+(c=='\r')+1;
			li.length=0;
			//li.pixs=0;
			}
		else
			++li.length;
		}
	lines.push_back(li);

	//printf("Podzielone po enterach, linii %u", lines.size());

// Przelot drugi, trimowanie i takie tam
	trim();

// Przeliczenie dlugosci linii
	//for(unsigned l=0u; l<lines.size(); ++l)
	for(std::list<LineInfo>::iterator it=lines.begin(); it!=lines.end(); ++it)
		{
		LineInfo& li=*it;//lines[l];

		li.pixs=getLineLen(li.start, li.length);
		}

// Wrapowanie, opcjonalne
	if(opts&WRAP)
		wrap();

	if(!(opts&OVERFLOWH))
		clip();

// Rzeczywiste wymiary
	rw=w;
	rh=lines.size()*line;

	if(opts&LEFT)	// Rzeczywisty wymiar moze byc mniejszy tylko dla wyrownania do lewej
		//for(unsigned i=0u; i<lines.size(); ++i)
		for(std::list<LineInfo>::iterator it=lines.begin(); it!=lines.end(); ++it)
			//rw=std::max(rw, lines[i].pixs);
			rw=std::max(rw, it->pixs);

// Przycinanie, opcjonalne
	if(!(opts&OVERFLOWV) && rh>h)
		{
		std::list<LineInfo>::iterator it=lines.begin();

		for(int i=0; i<h/line; ++i, ++it);

		lines.erase( it, lines.end() );
		rh=lines.size()*line;
		}

// Ponowne trimowanie i finalne przeliczenie dlugosci linii
// Tylko, jesli bylo wrapowane lub przycinane
	if((opts&WRAP) || !(opts&OVERFLOW))
		{
		trim();

	// Finalne przeliczenie dlugosci linii
		//for(unsigned l=0u; l<lines.size(); ++l)
		for(std::list<LineInfo>::iterator it=lines.begin(); it!=lines.end(); ++it)
			{
			LineInfo& li=*it;//lines[l];

			li.pixs=getLineLen(li.start, li.length);
			}

		rh=lines.size()*line;

		rw=w;
		if(opts&LEFT)	// Rzeczywisty wymiar moze byc mniejszy tylko dla wyrownania do lewej
			//for(unsigned i=0u; i<lines.size(); ++i)
			for(std::list<LineInfo>::iterator it=lines.begin(); it!=lines.end(); ++it)
				//rw=std::max(rw, lines[i].pixs);
				rw=std::max(rw, it->pixs);
		}

// Wypisanie~
	/*Log::info("Linii: %d; w %d h %d rw %d rh %d", lines.size(), w, h, rw, rh);
	//for(unsigned l=0; l<lines.size(); ++l)
	for(std::list<LineInfo>::iterator it=lines.begin(); it!=lines.end(); ++it)
		{
		//Log::info("%u: start %d len %d pixs %d; first 0x%02X last 0x%02X", l, lines[l].start, lines[l].length, lines[l].pixs, str[lines[l].start], str[lines[l].start+lines[l].length]);

		LineInfo& li=*it;//lines[l];
		fprintf(stdout, "\"");
		for(int i=li.start; i<li.start+li.length; ++i)
			fprintf(stdout, "%c", str[i]);
		fprintf(stdout, "\" %d\n", li.pixs);
		}*/
	}

void Text::print()
	{
	int x;
	int y;

	if(opts&TOP)			y=h;
	else if(opts&MIDDLE)	y=h-(h-rh)/2;
	else if(opts&BOTTOM)	y=rh;
	else					y=h;

	Render::Camera cam;
	cam.GUI(w, h);

	//LOG_DEBUG("Text.print: [%s]", str.c_str());
	Render::getInstance().statePush();
	Render::getInstance().setFrameBuffer(buff, true);
	Render::getInstance().setCamera(cam);
	Render::getInstance().setColor(Vector(1, 1, 1, alpha));

	for(std::list<LineInfo>::iterator it=lines.begin(); it!=lines.end(); ++it)
		{
		LineInfo& li=*it;

		x=0;
		if(opts&LEFT)			x=0;
		else if(opts&CENTER)	x=(w-li.pixs)/2;
		else if(opts&RIGHT)		x= w-li.pixs;

		for(int i=li.start; i<li.start+li.length; ++i)
			{
			unsigned char c=str[i];

			if(c>' ')// && c<font->getAnims()+' ')
				{
				Render::getInstance().draw(Orientation(Vector(x, y, 0), Orientation::FLAT_XY), font->getAnimation(c-' ').getDirection(Sprite::DirectionName::DEFAULT).getFrame(0u).getImage());
				}

			x+=getCharLen(c, x);

			//printf("     [%d][%c]", (unsigned char)c, (char)c);
			}

		y-=line;
		}

	Render::getInstance().statePop();
	}


void Text::trim()
	{
	//for(unsigned l=0u; l<lines.size(); ++l)
	for(std::list<LineInfo>::iterator it=lines.begin(); it!=lines.end(); ++it)
		{
		LineInfo& li=*it;//lines[l];

		if(opts&(RIGHT|CENTER))
			{
			for(int i=li.start; i<li.start+li.length; ++i)
				{
				if((unsigned char)str[i]>' ')
					break;

				++li.start;
				--li.length;
				}
			}

		if(opts&(LEFT|CENTER))
			{
			for(int i=li.start+li.length-1; i>li.start; --i)
				{
				if((unsigned char)str[i]>' ')
					break;

				--li.length;
				}
			}
		}
	}

void Text::wrap()
	{
	//for(unsigned l=0u; l<lines.size(); ++l)
	for(std::list<LineInfo>::iterator it=lines.begin(); it!=lines.end(); ++it)
		{
		LineInfo& li=*it;//lines[l];

		int pixs=0;

		for(int i=li.start; i<li.start+li.length; ++i)
			{
			unsigned char c=str[i];

			pixs+=getCharLen(c, pixs);

			//if(c<=' ' && (pixs>w || pixs+getWordLen(i+1, li.length-i+li.start-1)>w))
			if(c<=' ' && pixs+getWordLen(i+1, li.length-i+li.start-1)>w)
				{
				LineInfo nl;

				nl.start=i;
				nl.length=li.length-i+li.start;

				li.length-=nl.length;

				for(int t=nl.start; t<nl.start+nl.length; ++t)
					if((unsigned char)str[t]<=' ')
						++nl.start,
						--nl.length;
					else
						break;

				it=lines.insert( ++it, nl );
				--it;
				break;
				}
			}
		}
	}

void Text::clip()
	{
	// todo Clipowanie tekstu
	//for(unsigned l=0u; l<lines.size(); ++l)
	for(std::list<LineInfo>::iterator it=lines.begin(); it!=lines.end(); ++it)
		{
		LineInfo& li=*it;//lines[l];

		int pixs=0;

		for(int i=li.start; i<li.start+li.length; ++i)
			{
			unsigned c=str[i]&0xFF;

			pixs+=getCharLen(c, pixs);

			if(pixs>w)
				{
				li.length=li.length-i+li.start;

				break;
				}
			}
		}
	}


int Text::getCharLen(unsigned char c, int pos)
	{
	/*
// Entery
	if(c=='\r')
		return -pos;
	else if(c=='\n')
		return 0;
// Taby
	else if(c=='\t')
		return (pos/tab+1)*tab-pos;
// Niedrukowalne
	else if(c<=' ')// || c>=font->getAnims()+' ')
		return space;
// Kazdy inny
	else
		return font->getAnim(c-' ').getDir(Graphics::DEFAULT).getFrame(0u).w+1;
	*/

	switch(c)
		{
		case '\r':
			return -pos;
		break;

		case '\n':
			return 0;
		break;

		case '\t':
			return (pos/tab+1)*tab-pos;
		break;

		default:
			if(c<=' ')
				return space;
			return font->getAnimation(c-' ').getDirection(Sprite::DirectionName::DEFAULT).getFrame(0u).getWidth()+1;
		break;
		}
	}

int Text::getWordLen(int start, int len)		// Zwraca dlugosc najblizszego slowa w pixelach
	{
	if(!font)
		return 0;

	int pixs=0;

	for(int i=start; i<start+len; ++i)
		{
		unsigned char c=str[i];

		if(c<=' ')// || c>=font->getAnims()+' ')
			break;

		pixs+=font->getAnimation(c-' ').getDirection(Sprite::DirectionName::DEFAULT).getFrame(0u).getWidth()+1;
		}

	return pixs;
	}

int Text::getLineLen(int start, int len)		// Zwraca dlugosc linii w pixelach
	{
	if(!font)
		return 0;

	int pixs=0;
	int maxpixs=0;

	for(int i=start; i<start+len; ++i)
		{
		unsigned char c=str[i];

		if(c=='\n')
			break;

		pixs+=getCharLen(c, pixs);

		maxpixs=std::max(pixs, maxpixs);
		}

	return maxpixs;
	}

