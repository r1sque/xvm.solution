#pragma once

#define COLORPICKER_WIDTH		20
#define COlORPICKER_HEIGHT		8
#define color_size 200

class Colorpicker : public Element {
public:
	__forceinline Colorpicker() : m_open{ false }, m_label{}, m_color{}, m_ptr{ nullptr } {
		m_flags = ElementFlags::DRAW | ElementFlags::CLICK | ElementFlags::ACTIVE | ElementFlags::SAVE | ElementFlags::DEACIVATE;
		m_type = ElementTypes::COLORPICKER;
		m_h = m_base_h = COlORPICKER_HEIGHT;
		m_use_label = true;
		m_show = true;
	}

	__forceinline void setup(const std::string& label, const std::string& file_id, Color color, Color* ptr = nullptr) {
		m_label = label;
		m_file_id = file_id;
		m_color = color;
		m_ptr = ptr;

		if (m_ptr)
			*m_ptr = m_color;
	}

	__forceinline void set(Color color) {
		bool changed = m_color.rgba() != color.rgba();

		m_color = color;

		if (m_ptr)
			*m_ptr = m_color;

		if (changed && m_callback)
			m_callback();
	}

	__forceinline Color get() {
		return m_color;
	}

	static void init();

	static __forceinline Color ColorFromPos(int x, int y) {
		return *(Color*)(gradient.get() + x + y * color_size);
	}

public:
	static int texture;
	static std::unique_ptr< Color[] > gradient;

protected:
	bool		m_open;
	std::string m_label;
	Color m_color;
	Color pointer_color;
	vec2_t ColorPos;
	vec2_t m_hue_pos;
	Color* m_ptr;
	float m_hue = 1.f;
	float m_saturation = 1.f;
	float m_value = 1.f;
	bool DoDragHue = false;
	bool DoDragCol = false;
	bool copy_paste;
	bool do_drag_alpha = false;
	vec2_t m_alpha_pos;
protected:
	void draw() override;
	void think() override;
	void click() override;
};