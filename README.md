[FontAwesome](http://fontawesome.io/) helper for c++
==================
Helper and generator to ease the use of FontAwesome in c++. The name of each icons are parsed from the css file and stay the same as in the original API:


```c++
#include "FontAwesome.h"

static std::vector<string> someIcons = {	
  	fa::glass(),
	fa::music(),
	fa::search(),
	fa::heart(),
	fa::star(),
	fa::user(),
	fa::film(),
	fa::check(),
	fa::remove(),
	fa::close(),
	fa::times(),
	fa::search_plus(),
	fa::search_minus(),
	fa::power_off(),
	fa::signal(),
	fa::gear(),
	fa::cog(),
	fa::trash_o(),
	fa::home(),
	fa::file_o(),
	fa::clock_o() 
};

void drawHeartIcon( const vec2 &position )
{
  string icon = fa::heart();
  drawString( icon, position );
}
```

If you want to re-generate the FontAwesome.h file, you can use the FontAwesomeGenerator and this line of code :

```c++
#include "FontAwesomeGenerator.h"

int main( int argc, char **argv )
{
  FontAwesomeGenerator::write( "pathTo/font-awesome.css", "otherPathTo/FontAwesome.h" );
}
```
