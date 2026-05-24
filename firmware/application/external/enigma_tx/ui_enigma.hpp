/*
 * Copyright (C) 2015 Jared Boone, ShareBrained Technology, Inc.
 * Copyright (C) 2016 Furrtek
 *
 * This file is part of PortaPack.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#ifndef __ENIGMA_TX_H__
#define __ENIGMA_TX_H__

#include "ui.hpp"
#include "ui_widget.hpp"
#include "ui_navigation.hpp"
#include "ui_transmitter.hpp"
#include "app_settings.hpp"
#include "radio_state.hpp"
#include "portapack.hpp"
#include "message.hpp"
#include "volume.hpp"
#include "audio.hpp"
#include "morse.hpp"
#include "enigma.hpp"

#include <ch.h>

using namespace morse;

namespace ui::external_app::enigma_tx {

enum Modulation {
    FM = 0,
    CW = 1
};

class EnigmaView : public View {
   public:
    EnigmaView(NavigationView& nav);
    ~EnigmaView();

    EnigmaView(const EnigmaView&) = delete;
    EnigmaView(EnigmaView&&) = delete;
    EnigmaView& operator=(const EnigmaView&) = delete;
    EnigmaView& operator=(EnigmaView&&) = delete;

    void focus() override;
    void paint(Painter& painter) override;

    void on_tx_progress(const uint32_t progress, const bool done);

    std::string title() const override { return "Enigma TX"; };

    uint32_t time_unit_ms{0};
    size_t symbol_count{0};

   private:
    NavigationView& nav_;
    std::string message{};
    uint32_t time_units{0};
    std::string tone_input_buffer{};  // Holds the tone value while the text prompt is open

    TxRadioState radio_state_{
        0 /* frequency */,
        1750000 /* bandwidth */,
        1536000 /* sampling rate */
    };

    std::string buffer{"PORTAPACK"};
    bool mode_cw{true};
    uint32_t speed{15};
    uint32_t tone{700};
    app_settings::SettingsManager settings_{
        "tx_enigma",
        app_settings::Mode::TX,
        {
            {"message"sv, &buffer},
            {"speed"sv, &speed},
            {"tone"sv, &tone},
            {"mode_cw"sv, &mode_cw},
        }};

    bool start_tx();
    void update_tx_duration();
    void on_set_tone(NavigationView& nav);
    void on_set_text(NavigationView& nav);

    Thread* ookthread{nullptr};
    bool run{false};

    Labels labels{
        {{4 * 8, 6 * 8}, "Speed:   wpm", Theme::getInstance()->fg_light->foreground},
        {{4 * 8, 8 * 8}, "Tone:    Hz", Theme::getInstance()->fg_light->foreground},
        {{4 * 8, 10 * 8}, "Modulation:", Theme::getInstance()->fg_light->foreground},
        {{1 * 8, 25 * 8}, "TX will last", Theme::getInstance()->fg_light->foreground}};


    NumberField field_speed{
        {10 * 8, 6 * 8},
        3,
        {10, 45},
        1,
        ' '};

    NumberField field_tone{
        {9 * 8, 8 * 8},
        4,
        {100, 9999},
        20,
        ' '};

    OptionsField options_modulation{
        {15 * 8, 10 * 8},
        2,
        {{"CW", Modulation::CW},
         {"FM", Modulation::FM}}};

    Text text_tx_duration{
        {14 * 8, 25 * 8, 4 * 8, 16},
        "-"};

    Text text_message{
        {1 * 8, 15 * 8, 28 * 8, 16},
        ""};

    Button button_message{
        {1 * 8, 17 * 8, 12 * 8, 28},
        "Set message"};

    ProgressBar progressbar{
        {2 * 8, 28 * 8, UI_POS_WIDTH_REMAINING(4), 16}};

    TransmitterView tx_view{
        (int16_t)UI_POS_Y_BOTTOM(4),
        10000,
        12};

    MessageHandlerRegistration message_handler_tx_progress{
        Message::ID::TXProgress,
        [this](const Message* const p) {
            const auto message = *reinterpret_cast<const TXProgressMessage*>(p);
            this->on_tx_progress(message.progress, message.done);
        }};
};

}  // namespace ui::external_app::morse_tx

#endif /*__ENIGMA_TX_H__*/
