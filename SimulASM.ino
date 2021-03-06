#include "./SimulASM.h"

// ============================================================================

void setup()
{
    delay(2500);
    Serial.begin(9600);
    pinMode(LED_BUILTIN, OUTPUT);

    for (auto address = 0; address < 4; ++address)
    {
        mem_display.shutdown(address, false);
        mem_display.setIntensity(address, 0);
        mem_display.clearDisplay(address);
    }

    reg_display.shutdown(0, false);
    reg_display.setIntensity(0, 0);
    reg_display.clearDisplay(0);

    module.setupDisplay(true, 0);
    wait_button(ENTER);

    lcd_i2c.init();
    lcd_i2c.backlight();
    lcd_i2c.setCursor(6, 1);
    lcd_i2c.print("SimulASM");
    lcd_i2c.setCursor(16, 3);
    lcd_i2c.print("2020");

    SR = -1;
}

// ============================================================================

void loop()
{
    for (auto&& keypad: cmd_pads)
    {
        auto key = keypad.getKey();
        if (!key) continue;

        module.setLEDs(key);

        auto aux = get_instruction(key);
        for (auto i = 0; i < aux.length(); ++i)
            segment_text[i] = aux[i];

        print_segment();

        switch (key)
        {
            case _RET: run_code();

            parse_op(PSH, psh);
            parse_op(POP, pop);
            parse_op(NOP, nop);
            parse_op(AND, and);
            parse_op(NOT, not);
            parse_op(XOR, xor);
            parse_op(ADD, add);
            parse_op(SUB, sub);
            parse_op(MUL, mul);
            parse_op(DIV, div);
            parse_op(INC, inc);
            parse_op(DEC, dec);
            parse_op(NEG, neg);
            parse_op(MOD, mod);
            parse_op(SHL, shl);
            parse_op(SHR, shr);
            parse_op(MOV, mov);
            parse_op(LBL, lbl);
            //parse_op(RET, ret);
            parse_op(CMP, cmp);
            parse_op(JMP, jmp);
            parse_op(JNE, jne);
            parse_op(JLE, jle);
            parse_op(JGE, jge);
            parse_op(JE, je);
            parse_op(JL, jl);
            parse_op(JG, jg);
            parse_op(JC, jc);
            parse_op(JO, jo);
            parse_op(JS, js);
            parse_op(JZ, jz);
            parse_op(OR, or);

            /*
            default:
            {
                wait_button(ENTER);
                break;
            }
            */
        }

        clear_segment();
    }

    call(set_memory_byte, 32);
    call(display_register, 8);
}

// ============================================================================
