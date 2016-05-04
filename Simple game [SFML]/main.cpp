#define _CRT_SECURE_NO_WARNINGS
#include "game_interface.h"



int main()
{
	//*
	Facade *game = new Facade();
	game->init_settings();
	game->init_entities();

	game->start_engine();
	
	delete game;
	//sf::RenderWindow window(sf::VideoMode(1280, 800), "menu test");
	//menu(window);//вызов меню
	return 0;
}
/*
#include <SFML/Graphics.hpp>
 
 
class FocusObject
{
public:
    virtual void setFocus () = 0 ;
    virtual void deleteFocus () = 0 ;
    virtual void event ( const sf::Event & ) = 0 ;
} ;
 
 bool write(false);
 
class TextBox : public sf::Drawable , public FocusObject 
{
public:
    TextBox ( const sf::Text & text ) : m_text ( text )
    {
        m_newText = m_text.getString() ;
        updateRect() ;
        m_box.setFillColor( sf::Color::Blue ) ;
    }
    virtual void draw ( sf::RenderTarget & render , sf::RenderStates states ) const
    {
        if ( m_textChanged )
        {
            const_cast<TextBox*>(this)->setText ( m_newText ) ;
 
            m_textChanged = false ;
        }
        render.draw ( m_box , states ) ;
        render.draw ( m_text , states ) ;
    }
    virtual void setFocus ()
    {
        m_box.setFillColor( sf::Color::Red ) ;
    }
    virtual void deleteFocus ()
    {
        m_box.setFillColor( sf::Color::Blue ) ;
    }
    virtual void event ( const sf::Event & event )
    {
        if (event.type == sf::Event::TextEntered)
        {
            //Обработка ввода
            m_textChanged = true ;
            switch ( event.text.unicode )
            {
            case 0xD: //Return
               // m_newText += L'\n' ;
				write = false;
                break ;
            case 0x8://Backspace
                if ( !m_newText.isEmpty() )
                    m_newText.erase(m_newText.getSize()-1) ;
                break ;
            default :
                {
                    m_newText += static_cast<wchar_t>(event.text.unicode) ;
                }
            }
        }
    }
    void setText ( const sf::String & str )
    {
        m_text.setString ( str ) ;
        updateRect() ;
    }
private:
    void updateRect ()
    {
        sf::FloatRect rect = m_text.getGlobalBounds() ;
        m_box.setPosition ( rect.left-5 , rect.top-5 ) ;
        m_box.setSize( sf::Vector2f(rect.width+10 , rect.height+10) ) ;
    }
    mutable sf::RectangleShape m_box ;
    mutable sf::Text m_text ;
    mutable sf::String m_newText ;
    mutable bool m_textChanged ;
} ;
 
 
 
class FocusController
{
public:
    FocusController ( FocusObject * obj = 0 ) : m_object (obj)
    {
        if ( m_object != 0 )
        {
            m_object->setFocus () ;
        }
    }
    void setFocusObject ( FocusObject * new_obj )
    {
        if ( m_object == new_obj )
            return ;
        if ( m_object != 0 )
            m_object->deleteFocus() ;
        if ( new_obj != 0 )
            new_obj->setFocus() ;
        m_object = new_obj ;
    }
    FocusObject * getFocusObject ( )
    {
        return m_object ;
    }
 
private:
    FocusObject * m_object ;
} ;
 
FocusController fc ;
 
 
 
int main()
{
    sf::RenderWindow app(sf::VideoMode(800, 600), L"SFML window");

    sf::Font font ;
    if ( !font.loadFromFile("Font/main_font.ttf") )
    {
        return EXIT_FAILURE ;
    }
    sf::Text txt ;
    txt.setPosition ( 50 , 50 ) ;
    txt.setFont ( font ) ;
    txt.setString ( L"Моя строка" ) ;
    int x = 0 ;
    TextBox tb ( txt ) ;

    while (app.isOpen())
    {
        sf::Event event;
        while (app.pollEvent(event))
        {
            // Close window : exit
            if (event.type == sf::Event::Closed)
                app.close();
			if (event.type == sf::Event::KeyPressed)
				if (event.key.code == sf::Keyboard::Escape)
				{
					write = true;
					break;
				}

        }
        app.clear(sf::Color(129, 181, 221));
        app.draw ( tb ) ;
        app.display();
    }
 
}
*/