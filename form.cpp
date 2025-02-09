#include "includes.h"

void Form::draw( ) {
	// opacity should reach 1 in 150 milliseconds.
	constexpr float frequency = 1.f / 0.15f;

	// the increment / decrement per frame.
	float step = frequency * g_csgo.m_globals->m_frametime;

	// if open		-> increment
	// if closed	-> decrement
	m_open ? m_opacity += step : m_opacity -= step;

	// clamp the opacity.
	math::clamp( m_opacity, 0.f, 1.f );

	m_alpha = 0xff * m_opacity;
	if( !m_alpha )
		return;

	// get gui color.
	Color color = g_gui.m_color;
	color.a( ) = m_alpha;

	// background.
	render::RoundedBoxStatic( m_x, m_y, m_width, m_height, 6, { 27, 27, 27, m_alpha } );
	/*for (int io = 0; io < m_width / 2 - 5; io++) {
		render::rect(m_x + 5 + (io * 2), m_y + 5, 1, m_height - 11, { 10, 10, 10, m_alpha }); // 20,20,20 - shit menu pattern
	}*/

	// border.
	render::RoundedBoxStaticOutline( m_x, m_y, m_width, m_height, 6, color);
	//render::rect( m_x + 1, m_y + 1, m_width - 2, m_height - 2, { 27, 27, 27, m_alpha });
	//render::rect( m_x + 2, m_y + 2, m_width - 4, m_height - 4, { 27, 27, 27, m_alpha });
	//render::rect( m_x + 3, m_y + 3, m_width - 6, m_height - 6, { 27, 27, 27, m_alpha });
	//render::rect( m_x + 4, m_y + 4, m_width - 8, m_height - 8, { 27, 27, 27, m_alpha });
	//render::rect( m_x + 5, m_y + 5, m_width - 10, m_height - 10, { 27, 27, 27, m_alpha });
	//render::rect(m_x + 6, m_y + 6, m_width - 11, m_height - 11, { 27, 27, 27, m_alpha });

	// draw tabs if we have any.
	if( !m_tabs.empty( ) ) {
		// tabs background and border.
		Rect tabs_area = GetTabsRect( );

		//render::rect_filled( tabs_area.x, tabs_area.y, tabs_area.w, tabs_area.h, { 27, 27, 27, m_alpha } );
		//render::rect( tabs_area.x, tabs_area.y, tabs_area.w, tabs_area.h, { 0, 0, 0, m_alpha } );
		//render::rect( tabs_area.x + 1, tabs_area.y + 1, tabs_area.w - 2, tabs_area.h - 2, { 48, 48, 48, m_alpha } );

		render::RoundedBoxStatic(tabs_area.x, tabs_area.y, tabs_area.w, tabs_area.h, 3, { 27, 27, 27, m_alpha });
		render::RoundedBoxStaticOutline(tabs_area.x, tabs_area.y, tabs_area.w, tabs_area.h, 3, { 0, 0, 0, m_alpha });
		render::RoundedBoxStaticOutline(tabs_area.x + 1, tabs_area.y + 1, tabs_area.w - 2, tabs_area.h - 2, 3, { 48, 48, 48, m_alpha });

		// Set a constant height for both the tabs and the elements.
		const float tab_height = 70.0f;
		const float element_height = m_height - tabs_area.y - tab_height;

		for (size_t i{}; i < m_tabs.size(); ++i) {
			const auto& t = m_tabs[i];

			// Define the clickable area for the tab.
			Rect tab_area{ tabs_area.x, tabs_area.y + (i * (tab_height + 1.7)), tabs_area.w, tab_height };

			// Check if the mouse is hovering over this tab.
			bool is_hovered = g_input.IsCursorInRect(tab_area);

			// Set the background color based on hover state, factoring in form opacity.
			Color background_color = is_hovered
				? Color{ 35, 35, 35, static_cast<int>(150 * m_opacity) }
			: Color{ 27, 27, 27, static_cast<int>(200 * m_opacity) };

			// Set border colors based on form opacity.
			Color border_color = Color{ 0, 0, 0, static_cast<int>(m_alpha) };
			Color inner_border_color = Color{ 48, 48, 48, static_cast<int>(m_alpha) };

			// Render the background rectangle.
			render::RoundedBoxStatic(tab_area.x, tab_area.y, tab_area.w, tab_area.h, 3, background_color);
			render::RoundedBoxStaticOutline(tab_area.x, tab_area.y, tab_area.w, tab_area.h, 3, border_color);
			render::RoundedBoxStaticOutline(tab_area.x + 1, tab_area.y + 1, tab_area.w - 2, tab_area.h - 2, 3, inner_border_color);

			// Set the text color based on whether the tab is active, factoring in form opacity.
			Color text_color = (t == m_active_tab)
				? Color{ color.r(), color.g(), color.b(), static_cast<int>(m_alpha) }
			: Color{ 152, 152, 152, static_cast<int>(m_alpha) };

			// Render the tab's title, vertically centered in the tab.
			render::menu_shade.string(tab_area.x + tab_area.w / 2, tab_area.y + tab_area.h / 2 - 8,
				text_color, t->m_title, render::ALIGN_CENTER);
		}

		// this tab has elements.
		if( !m_active_tab->m_elements.empty( ) ) {
			// elements background and border.
			Rect el = GetElementsRect( );

			//render::rect_filled( el.x, el.y, el.w, el.h, { 27, 27, 27, m_alpha } );
			//render::rect( el.x, el.y, el.w, el.h, { 0, 0, 0, m_alpha } );
			//render::rect( el.x + 1, el.y + 1, el.w - 2, el.h - 2, { 48, 48, 48, m_alpha } );

			render::RoundedBoxStatic(el.x, el.y, el.w, el.h, 3, { 27, 27, 27, m_alpha });
			render::RoundedBoxStaticOutline(el.x, el.y, el.w, el.h, 3, { 0, 0, 0, m_alpha });
			render::RoundedBoxStaticOutline(el.x + 1, el.y + 1, el.w - 2, el.h - 2, 3, { 48, 48, 48, m_alpha });


			std::string date = XOR(__DATE__);
			std::string text = tfm::format(XOR("%s | %s | %s "), date.c_str(), g_cl.m_user, g_cl.m_version);
			render::menu_shade.string(el.x + el.w - 5, el.y + el.h - 20, { 205, 205, 205, m_alpha }, text, render::ALIGN_RIGHT);

			// iterate elements to display.
			for( const auto& e : m_active_tab->m_elements ) {

				// draw the active element last.
				if( !e || ( m_active_element && e == m_active_element ) )
					continue;

				if( !e->m_show )
					continue;

				// this element we dont draw.
				if( !( e->m_flags & ElementFlags::DRAW ) )
					continue;

				e->draw( );
			}

			// we still have to draw one last fucker.
			if( m_active_element && m_active_element->m_show )
				m_active_element->draw( );
		}
	}
}