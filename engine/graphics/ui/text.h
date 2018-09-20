#pragma once

#include <string>
//#include <vector>
#include <list>

#include "../spriteptr.h"
#include "../../math/orientation.h"
#include "../../render/camera.h"
#include "../../render/framebuffer.h"

#ifdef OVERFLOW
#undef OVERFLOW
#endif

namespace Engine
	{
	namespace Graphics
		{
		namespace UI
			{
			class Text
				{
				public:
					enum Options
						{
					// Wyrownanie w poziomie
						LEFT=		0x1<<0,
						CENTER=		0x1<<1,
						RIGHT=		0x1<<2,
					// Wyrownanie w pionie
						TOP=		0x1<<3,
						MIDDLE=		0x1<<4,
						BOTTOM=		0x1<<5,

					// Zawijanie
						WRAP=		0x1<<6,					// Zawijanie tekstu

						CLIP=		0x1<<7,					// Przyciecie do max wymiarow
						OVERFLOWH=	0x1<<8,					// Moze wyleciec w poziomie
						OVERFLOWV=	0x1<<9,					// Moze wyleciec w pionie
						OVERFLOW=	(OVERFLOWH|OVERFLOWV),	// Moze wyleciec w dowolnym wymiarze

						DEFAULT=(LEFT|TOP|WRAP|OVERFLOW),		// Domyslne wartosci
						};

				private:
					struct LineInfo
						{
						int start, length, pixs;			// poczatek, liczba znakow, dlugosc w pikselach
						};

					int getCharLen(unsigned char c, int pos=0);
					int getWordLen(int start, int len);		// Dlugosc wyrazu w pikselach
					int getLineLen(int start, int len);		// Dlugosc linii w pikselach

					void parse();							// Wypelnienie info o liniach
					void print();							// Wyrysowanie do bufora zgodnie z tym co w lines'ach

					void trim();
					void wrap();
					void clip();

					bool upd;

					int w, h;								// Docelowe
					int rw, rh;								// Rzeczywiste

					int line, space, tab;					// Wymiary linii (wysokosc), spacji i taba (rozstaw kolumn, co ile pikseli)
					float alpha;

					std::string str;

					std::list<LineInfo> lines;

					Options opts;

					Graphics::SpritePtr font;
					Render::FrameBuffer buff;

				public:
					 Text();
					 Text(const std::string& font, const std::string& str, int w, int h, int line=-1, int space=-1, int tab=-1);
					// Text(const Text& txt);
					virtual ~Text() {clear();}

					Text& operator= (const std::string& str) {setStr(str); return *this;}
					Text& operator+=(const std::string& str) {addStr(str); return *this;}

					bool init(const std::string& font, const std::string& str, int w, int h, int line=-1, int space=-1, int tab=-1);
					bool init();
					void update();
					void print(const Math::Orientation& orientation) const;
					void clear();

					void setStr(const std::string& str);
					void addStr(const std::string& str);

					void setFont(const std::string& font);
					void setSpr(const std::string& font) {setFont(font);}

					void setOptions(Options sopts) {opts=sopts; upd=true;}	// Nie jest to zbyt bezpieczne. Mozna zrobic rozne, dziwne kombinacje~
					void setAlignLeft()		{opts=(Options)((opts&~(CENTER|RIGHT))|LEFT); upd=true;}
					void setAlignCenter() 	{opts=(Options)((opts&~(LEFT|RIGHT))|CENTER); upd=true;}
					void setAlignRight()	{opts=(Options)((opts&~(LEFT|CENTER))|RIGHT); upd=true;}
					void setAlignTop()		{opts=(Options)((opts&~(MIDDLE|BOTTOM))|TOP); upd=true;}
					void setAlignMiddle() 	{opts=(Options)((opts&~(TOP|BOTTOM))|MIDDLE); upd=true;}
					void setAlignBottom()	{opts=(Options)((opts&~(TOP|MIDDLE))|BOTTOM); upd=true;}
					void setWrapOn()		{opts=(Options) (opts| WRAP); upd=true;}
					void setWrapOff()		{opts=(Options) (opts&~WRAP); upd=true;}
					void setClip()			{opts=(Options)((opts&~OVERFLOW)|CLIP); upd=true;}
					void setOverflow()		{opts=(Options)((opts&~CLIP)|OVERFLOW); upd=true;}
					void setOverflow(bool v, bool h)	{opts=(Options)( (opts&~(CLIP|OVERFLOW)) | (v?OVERFLOWV:0) | (h?OVERFLOWH:0) | (!v&&!h?CLIP:0) ); upd=true;}
					void setW(int s)		{w=s; upd=true;}
					void setH(int s)		{h=s; upd=true;}
					void setDimension(int sw, int sh)	{w=sw; h=sh; upd=true;}
					void setAlpha(float sa)	{alpha=sa; print();}

					inline int getW() const {return w;}
					inline int getH() const {return h;}
					inline int getRealW() const {return rw;}
					inline int getRealH() const {return rh;}
					inline bool isEmpty() const {return str.empty();}
					inline std::string getStr() const {return str;}
					inline int getOptions() const {return opts;}
					inline float getAlpha() const {return alpha;}

					SpritePtr getFont() {return font;}
				};
			}
		}
	}
