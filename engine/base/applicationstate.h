/*
 * applicationstate.h
 *
 *  Created on: 2 mar 2017
 *      Author: crm
 */

#pragma once

#include <string>

#include "../core/application.h"

namespace Engine
	{
	/*namespace Core
		{
		class Application;
		}*/

	namespace Base
		{
		class ApplicationState
			{
			/*public:
				enum {
					FLAG_NONE=              0x0,
					FLAG_UPDATE_LOWER_STATE=0x1<<0,
					FLAG_PRINT_LOWER_STATE= 0x1<<1,
					};*/

			protected:
				Core::Application *application;

			public:
				ApplicationState(): application(nullptr) {}
				virtual ~ApplicationState() {}

				/** Inicjalizacja
				 * Wołana przez Application przy wrzucaniu stanu na stos
				 */
				virtual bool init(Core::Application *application)=0;
				/** Aktualizacja
				 * Wołane przez Application co klatkę lub dowolny, bliżej nieokreślony czas
				 * @return Czy powinny być aktualizowane stany poniżej?
				 */
				virtual bool update(float dt)=0;
				/** Wyświetlenie
				 * Wołane przez Application co jakiś czas, podobnie do update
				 * @return Czy powinny być wyświetlone stany poniżej?
				 */
				virtual void print(float tinterp)=0;
				virtual bool isOverlay() const=0;
				/** Czyszczenie
				 * Wywoływane przez Application przy zrzucaniu ze stosu stan�w
				 */
				virtual void clear()=0;

				/** Pauza
				 * Wywoływane przez Application gdy wrzucany jest nowy stan
				 */
				virtual void pause()=0;
				/** Wznowienie
				 * Wywoływane przez Application gdy stan wraca na wierzch stosu
				 */
				virtual void resume()=0;

				/** Identyfikator stanu
				 *
				 */
				//virtual int getId() const=0;
				virtual std::string getName() const=0;
			};

		} /* namespace Core */
	} /* namespace Engine */
