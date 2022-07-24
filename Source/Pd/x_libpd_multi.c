/*
 // Copyright (c) 2015-2018 Pierre Guillot.
 // For information on usage and redistribution, and for a DISCLAIMER OF ALL
 // WARRANTIES, see the file, "LICENSE.txt," in this distribution.
 */

#include <m_pd.h>
#include <s_net.h>
#include "x_libpd_multi.h"

//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////

static t_class *libpd_multi_receiver_class;

typedef struct _libpd_multi_receiver
{
    t_object    x_obj;
    t_symbol*   x_sym;
    void*       x_ptr;
    
    t_libpd_multi_banghook      x_hook_bang;
    t_libpd_multi_floathook     x_hook_float;
    t_libpd_multi_symbolhook    x_hook_symbol;
    t_libpd_multi_listhook      x_hook_list;
    t_libpd_multi_messagehook   x_hook_message;
} t_libpd_multi_receiver;

static void libpd_multi_receiver_bang(t_libpd_multi_receiver *x)
{
    if(x->x_hook_bang)
        x->x_hook_bang(x->x_ptr, x->x_sym->s_name);
}

static void libpd_multi_receiver_float(t_libpd_multi_receiver *x, t_float f)
{
    if(x->x_hook_float)
        x->x_hook_float(x->x_ptr, x->x_sym->s_name, f);
}

static void libpd_multi_receiver_symbol(t_libpd_multi_receiver *x, t_symbol *s)
{
    if(x->x_hook_symbol)
        x->x_hook_symbol(x->x_ptr, x->x_sym->s_name, s->s_name);
}

static void libpd_multi_receiver_list(t_libpd_multi_receiver *x, t_symbol *s, int argc, t_atom *argv)
{
    if(x->x_hook_list)
        x->x_hook_list(x->x_ptr, x->x_sym->s_name, argc, argv);
}

static void libpd_multi_receiver_anything(t_libpd_multi_receiver *x, t_symbol *s, int argc, t_atom *argv)
{
    if(x->x_hook_message)
        x->x_hook_message(x->x_ptr, x->x_sym->s_name, s->s_name, argc, argv);
}

static void libpd_multi_receiver_free(t_libpd_multi_receiver *x)
{
    pd_unbind(&x->x_obj.ob_pd, x->x_sym);
}

static void libpd_multi_receiver_setup(void)
{
    sys_lock();
    libpd_multi_receiver_class = class_new(gensym("libpd_multi_receiver"), (t_newmethod)NULL, (t_method)libpd_multi_receiver_free,
                                           sizeof(t_libpd_multi_receiver), CLASS_DEFAULT, A_NULL, 0);
    class_addbang(libpd_multi_receiver_class, libpd_multi_receiver_bang);
    class_addfloat(libpd_multi_receiver_class,libpd_multi_receiver_float);
    class_addsymbol(libpd_multi_receiver_class, libpd_multi_receiver_symbol);
    class_addlist(libpd_multi_receiver_class, libpd_multi_receiver_list);
    class_addanything(libpd_multi_receiver_class, libpd_multi_receiver_anything);
    sys_unlock();
}

void* libpd_multi_receiver_new(void* ptr, char const *s,
                               t_libpd_multi_banghook hook_bang,
                               t_libpd_multi_floathook hook_float,
                               t_libpd_multi_symbolhook hook_symbol,
                               t_libpd_multi_listhook hook_list,
                               t_libpd_multi_messagehook hook_message)
{
    
    t_libpd_multi_receiver *x = (t_libpd_multi_receiver *)pd_new(libpd_multi_receiver_class);
    if(x)
    {
        sys_lock();
        x->x_sym = gensym(s);
        sys_unlock();
        pd_bind(&x->x_obj.ob_pd, x->x_sym);
        x->x_ptr = ptr;
        x->x_hook_bang = hook_bang;
        x->x_hook_float = hook_float;
        x->x_hook_symbol = hook_symbol;
        x->x_hook_list = hook_list;
        x->x_hook_message = hook_message;
    }
    return x;
}

//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////

static t_class *libpd_multi_midi_class;

typedef struct _libpd_multi_midi
{
    t_object    x_obj;
    void*       x_ptr;
    
    t_libpd_multi_noteonhook            x_hook_noteon;
    t_libpd_multi_controlchangehook     x_hook_controlchange;
    t_libpd_multi_programchangehook     x_hook_programchange;
    t_libpd_multi_pitchbendhook         x_hook_pitchbend;
    t_libpd_multi_aftertouchhook        x_hook_aftertouch;
    t_libpd_multi_polyaftertouchhook    x_hook_polyaftertouch;
    t_libpd_multi_midibytehook          x_hook_midibyte;
} t_libpd_multi_midi;

static void libpd_multi_noteon(int channel, int pitch, int velocity)
{
    t_libpd_multi_midi* x = (t_libpd_multi_midi*)gensym("#libpd_multi_midi")->s_thing;
    if(x && x->x_hook_noteon)
    {
        x->x_hook_noteon(x->x_ptr, channel, pitch, velocity);
    }
}

static void libpd_multi_controlchange(int channel, int controller, int value)
{
    t_libpd_multi_midi* x = (t_libpd_multi_midi*)gensym("#libpd_multi_midi")->s_thing;
    if(x && x->x_hook_controlchange)
    {
        x->x_hook_controlchange(x->x_ptr, channel, controller, value);
    }
}

static void libpd_multi_programchange(int channel, int value)
{
    t_libpd_multi_midi* x = (t_libpd_multi_midi*)gensym("#libpd_multi_midi")->s_thing;
    if(x && x->x_hook_programchange)
    {
        x->x_hook_programchange(x->x_ptr, channel, value);
    }
}

static void libpd_multi_pitchbend(int channel, int value)
{
    t_libpd_multi_midi* x = (t_libpd_multi_midi*)gensym("#libpd_multi_midi")->s_thing;
    if(x && x->x_hook_pitchbend)
    {
        x->x_hook_pitchbend(x->x_ptr, channel, value);
    }
}

static void libpd_multi_aftertouch(int channel, int value)
{
    t_libpd_multi_midi* x = (t_libpd_multi_midi*)gensym("#libpd_multi_midi")->s_thing;
    if(x && x->x_hook_aftertouch)
    {
        x->x_hook_aftertouch(x->x_ptr, channel, value);
    }
}

static void libpd_multi_polyaftertouch(int channel, int pitch, int value)
{
    t_libpd_multi_midi* x = (t_libpd_multi_midi*)gensym("#libpd_multi_midi")->s_thing;
    if(x && x->x_hook_polyaftertouch)
    {
        x->x_hook_polyaftertouch(x->x_ptr, channel, pitch, value);
    }
}

static void libpd_multi_midibyte(int port, int byte)
{
    t_libpd_multi_midi* x = (t_libpd_multi_midi*)gensym("#libpd_multi_midi")->s_thing;
    if(x && x->x_hook_midibyte)
    {
        x->x_hook_midibyte(x->x_ptr, port, byte);
    }
}

static void libpd_multi_midi_free(t_libpd_multi_midi *x)
{
    pd_unbind(&x->x_obj.ob_pd, gensym("#libpd_multi_midi"));
}

static void libpd_multi_midi_setup(void)
{
    sys_lock();
    libpd_multi_midi_class = class_new(gensym("libpd_multi_midi"), (t_newmethod)NULL, (t_method)libpd_multi_midi_free,
                                       sizeof(t_libpd_multi_midi), CLASS_DEFAULT, A_NULL, 0);
    sys_unlock();
}

void* libpd_multi_midi_new(void* ptr,
                           t_libpd_multi_noteonhook hook_noteon,
                           t_libpd_multi_controlchangehook hook_controlchange,
                           t_libpd_multi_programchangehook hook_programchange,
                           t_libpd_multi_pitchbendhook hook_pitchbend,
                           t_libpd_multi_aftertouchhook hook_aftertouch,
                           t_libpd_multi_polyaftertouchhook hook_polyaftertouch,
                           t_libpd_multi_midibytehook hook_midibyte)
{
    
    t_libpd_multi_midi *x = (t_libpd_multi_midi *)pd_new(libpd_multi_midi_class);
    if(x)
    {
        sys_lock();
        t_symbol* s = gensym("#libpd_multi_midi");
        sys_unlock();
        pd_bind(&x->x_obj.ob_pd, s);
        x->x_ptr = ptr;
        x->x_hook_noteon        = hook_noteon;
        x->x_hook_controlchange = hook_controlchange;
        x->x_hook_programchange = hook_programchange;
        x->x_hook_pitchbend     = hook_pitchbend;
        x->x_hook_aftertouch    = hook_aftertouch;
        x->x_hook_polyaftertouch= hook_polyaftertouch;
        x->x_hook_midibyte      = hook_midibyte;
    }
    return x;
}

//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////

static t_class *libpd_multi_print_class;

typedef struct _libpd_multi_print
{
    t_object    x_obj;
    void*       x_ptr;
    t_libpd_multi_printhook x_hook;
} t_libpd_multi_print;

static void libpd_multi_print(const char* message)
{
    t_libpd_multi_print* x = (t_libpd_multi_print*)gensym("#libpd_multi_print")->s_thing;
    if(x && x->x_hook)
    {
        x->x_hook(x->x_ptr, message);
    }
}

static void libpd_multi_print_setup(void)
{
    sys_lock();
    libpd_multi_print_class = class_new(gensym("libpd_multi_print"), (t_newmethod)NULL, (t_method)NULL,
                                       sizeof(t_libpd_multi_print), CLASS_DEFAULT, A_NULL, 0);
    sys_unlock();
}

void* libpd_multi_print_new(void* ptr, t_libpd_multi_printhook hook_print)
{
    
    t_libpd_multi_print *x = (t_libpd_multi_print *)pd_new(libpd_multi_print_class);
    if(x)
    {
        sys_lock();
        t_symbol* s = gensym("#libpd_multi_print");
        sys_unlock();
        pd_bind(&x->x_obj.ob_pd, s); 
        x->x_ptr = ptr;
        x->x_hook = hook_print;
    }
    return x;
}

//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
//

// font char metric triples: pointsize width(pixels) height(pixels)
static int defaultfontshit[] = {
    8,  5,  11,  10, 6,  13,  12, 7,  16,  16, 10, 19,  24, 14, 29,  36, 22, 44,
    16, 10, 22,  20, 12, 26,  24, 14, 32,  32, 20, 38,  48, 28, 58,  72, 44, 88
}; // normal & zoomed (2x)
#define NDEFAULTFONT (sizeof(defaultfontshit)/sizeof(*defaultfontshit))

static void libpd_defaultfont_init(void)
{
    int i;
    t_atom zz[NDEFAULTFONT+2];
    SETSYMBOL(zz, gensym("."));
    SETFLOAT(zz+1,0);
    for (i = 0; i < (int)NDEFAULTFONT; i++)
    {
        SETFLOAT(zz+i+2, defaultfontshit[i]);
    }
    pd_typedmess(gensym("pd")->s_thing, gensym("init"), 2+NDEFAULTFONT, zz);
}

//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////

void pd_tilde_setup(void);

void above_tilde_setup();
void add_tilde_setup();
void adsr_tilde_setup();
void setup_allpass0x2e2nd_tilde();
void setup_allpass0x2erev_tilde();
void args_setup();
void asr_tilde_setup();
void autofade_tilde_setup();
void autofade2_tilde_setup();
void balance_tilde_setup();
void bandpass_tilde_setup();
void bandstop_tilde_setup();
void setup_bend0x2ein();
void setup_bend0x2eout();
void setup_bl0x2eosc_tilde();
void bicoeff_setup();
void biquads_tilde_setup();
void blocksize_tilde_setup();
void break_setup();
void brown_tilde_setup();
void buffer_setup();
void button_setup();
void setup_canvas0x2eactive();
void setup_canvas0x2ebounds();
void setup_canvas0x2eedit();
void setup_canvas0x2egop();
void setup_canvas0x2emouse();
void setup_canvas0x2ename();
void setup_canvas0x2epos();
void setup_canvas0x2esetname();
void setup_canvas0x2evis();
void setup_canvas0x2ezoom();
void setup_canvas0x2efile();
void ceil_setup();
void ceil_tilde_setup();
void cents2ratio_setup();
void cents2ratio_tilde_setup();
void chance_setup();
void chance_tilde_setup();
void changed_setup();
void changed_tilde_setup();
void changed2_tilde_setup();
void click_setup();
void clipnoise_tilde_setup();
void cmul_tilde_setup();
void colors_setup();
void setup_comb0x2efilt_tilde();
void setup_comb0x2erev_tilde();
void cosine_tilde_setup();
void crackle_tilde_setup();
void crossover_tilde_setup();
void setup_ctl0x2ein();
void setup_ctl0x2eout();
void cusp_tilde_setup();
void decay_tilde_setup();
void decay2_tilde_setup();
void default_setup();
void del_tilde_setup();
void detect_tilde_setup();
void dir_setup();
void dollsym_setup();
void downsample_tilde_setup();
void drive_tilde_setup();
void dust_tilde_setup();
void dust2_tilde_setup();
void else_setup();
void envgen_tilde_setup();
void eq_tilde_setup();
void factor_setup();
void fader_tilde_setup();
void fbdelay_tilde_setup();
void fbsine_tilde_setup();
void fbsine2_tilde_setup();
void setup_fdn0x2erev_tilde();
void ffdelay_tilde_setup();
void f2s_tilde_setup();
void float2bits_setup();
void float2sig_tilde_setup();
void floor_setup();
void floor_tilde_setup();
void fold_setup();
void fold_tilde_setup();
void fontsize_setup();
void format_setup();
void setup_freq0x2eshift_tilde();
void function_setup();
void function_tilde_setup();
void gate2imp_tilde_setup();
void gaussian_tilde_setup();
void gbman_tilde_setup();
void gcd_setup();
void setup_giga0x2erev_tilde();
void glide_tilde_setup();
void glide2_tilde_setup();
void gray_tilde_setup();
void gui_setup();
void henon_tilde_setup();
void highpass_tilde_setup();
void highshelf_tilde_setup();
void hot_setup();
void hz2rad_setup();
void ikeda_tilde_setup();
void imp_tilde_setup();
void imp2_tilde_setup();
void impseq_tilde_setup();
void impulse_tilde_setup();
void impulse2_tilde_setup();
void initmess_setup();
void keyboard_setup();
void lag_tilde_setup();
void lag2_tilde_setup();
void lastvalue_tilde_setup();
void latoocarfian_tilde_setup();
void lb_setup();
void lfnoise_tilde_setup();
void limit_setup();
void lincong_tilde_setup();
void loadbanger_setup();
void logistic_tilde_setup();
void loop_setup();
void lop2_tilde_setup();
void lorenz_tilde_setup();
void lowpass_tilde_setup();
void lowshelf_tilde_setup();
void match_tilde_setup();
void median_tilde_setup();
void merge_setup();
void message_setup();
void messbox_setup();
void metronome_setup();
void midi_setup();
void mouse_setup();
void setup_mov0x2eavg_tilde();
void setup_mov0x2erms_tilde();
void mtx_tilde_setup();
void note_setup();
void setup_note0x2ein();
void setup_note0x2eout();
void noteinfo_setup();
void nyquist_tilde_setup();
void op_tilde_setup();
void openfile_setup();
void oscope_tilde_setup();
void pack2_setup();
void pad_setup();
void pan2_tilde_setup();
void pan4_tilde_setup();
void panic_setup();
void parabolic_tilde_setup();
void peak_tilde_setup();
void setup_pgm0x2ein();
void setup_pgm0x2eout();
void pic_setup();
void pimp_tilde_setup();
void pimpmul_tilde_setup();
void pinknoise_tilde_setup();
void pluck_tilde_setup();
void pmosc_tilde_setup();
void power_tilde_setup();
void properties_setup();
void pulse_tilde_setup();
void pulsecount_tilde_setup();
void pulsediv_tilde_setup();
void quad_tilde_setup();
void quantizer_setup();
void quantizer_tilde_setup();
void rad2hz_setup();
void ramp_tilde_setup();
void rampnoise_tilde_setup();
void setup_rand0x2ef();
void setup_rand0x2ef_tilde();
void setup_rand0x2ei();
void setup_rand0x2ei_tilde();
void setup_rand0x2eseq();
#if ENABLE_SFONT
void sfont_tilde_setup();
#endif
void randpulse_tilde_setup();
void randpulse2_tilde_setup();
void range_tilde_setup();
void ratio2cents_setup();
void ratio2cents_tilde_setup();
void rec_setup();
void receiver_setup();
void rescale_setup();
void rescale_tilde_setup();
void resonant_tilde_setup();
void resonant2_tilde_setup();
void retrieve_setup();
void rint_setup();
void rint_tilde_setup();
void rms_tilde_setup();
void rotate_tilde_setup();
void routeall_setup();
void router_setup();
void routetype_setup();
void s2f_tilde_setup();
void saw_tilde_setup();
void saw2_tilde_setup();
void schmitt_tilde_setup();
void selector_setup();
void separate_setup();
void sequencer_tilde_setup();
void sh_tilde_setup();
void shaper_tilde_setup();
void sig2float_tilde_setup();
void sin_tilde_setup();
void sine_tilde_setup();
void slew_tilde_setup();
void slew2_tilde_setup();
void slice_setup();
void sort_setup();
void spread_setup();
void spread_tilde_setup();
void square_tilde_setup();
void sr_tilde_setup();
void standard_tilde_setup();
void status_tilde_setup();
void stepnoise_tilde_setup();
void susloop_tilde_setup();
void suspedal_setup();
void svfilter_tilde_setup();
void symbol2any_setup();
void table_tilde_setup();
void tabplayer_tilde_setup();
void tabwriter_tilde_setup();
void tempo_tilde_setup();
void setup_timed0x2egate_tilde();
void toggleff_tilde_setup();
void setup_touch0x2ein();
void setup_touch0x2eout();
void tri_tilde_setup();
void setup_trig0x2edelay_tilde();
void setup_trig0x2edelay2_tilde();
void trighold_tilde_setup();
void trunc_setup();
void trunc_tilde_setup();
void unmerge_setup();
void voices_setup();
void vsaw_tilde_setup();
void vu_tilde_setup();
void wt_tilde_setup();
void wavetable_tilde_setup();
void wrap2_setup();
void wrap2_tilde_setup();
void xfade_tilde_setup();
void xgate_tilde_setup();
void xgate2_tilde_setup();
void xmod_tilde_setup();
void xmod2_tilde_setup();
void xselect_tilde_setup();
void xselect2_tilde_setup();
void zerocross_tilde_setup();

void libpd_multi_init(void)
{
    static int initialized = 0;
    if(!initialized)
    {
        libpd_set_noteonhook(libpd_multi_noteon);
        libpd_set_controlchangehook(libpd_multi_controlchange);
        libpd_set_programchangehook(libpd_multi_programchange);
        libpd_set_pitchbendhook(libpd_multi_pitchbend);
        libpd_set_aftertouchhook(libpd_multi_aftertouch);
        libpd_set_polyaftertouchhook(libpd_multi_polyaftertouch);
        libpd_set_midibytehook(libpd_multi_midibyte);
        libpd_set_printhook(libpd_multi_print);
        
        libpd_set_verbose(0);
        libpd_init();
        pd_tilde_setup();
        libpd_multi_receiver_setup();
        libpd_multi_midi_setup();
        libpd_multi_print_setup();
        libpd_defaultfont_init();
        libpd_set_verbose(4);
        
        socket_init();

        above_tilde_setup();
        add_tilde_setup();
        adsr_tilde_setup();
        setup_allpass0x2e2nd_tilde();
        setup_allpass0x2erev_tilde();
        args_setup();
        asr_tilde_setup();
        autofade_tilde_setup();
        autofade2_tilde_setup();
        balance_tilde_setup();
        bandpass_tilde_setup();
        bandstop_tilde_setup();
        setup_bend0x2ein();
        setup_bend0x2eout();
        setup_bl0x2eosc_tilde();
        bicoeff_setup();
        biquads_tilde_setup();
        blocksize_tilde_setup();
        break_setup();
        brown_tilde_setup();
        buffer_setup();
        button_setup();
        setup_canvas0x2eactive();
        setup_canvas0x2ebounds();
        setup_canvas0x2eedit();
        setup_canvas0x2egop();
        setup_canvas0x2emouse();
        setup_canvas0x2ename();
        setup_canvas0x2epos();
        setup_canvas0x2esetname();
        setup_canvas0x2evis();
        setup_canvas0x2ezoom();
        setup_canvas0x2efile();
        ceil_setup();
        ceil_tilde_setup();
        cents2ratio_setup();
        cents2ratio_tilde_setup();
        chance_setup();
        chance_tilde_setup();
        changed_setup();
        changed_tilde_setup();
        changed2_tilde_setup();
        click_setup();
        clipnoise_tilde_setup();
        cmul_tilde_setup();
        colors_setup();
        setup_comb0x2efilt_tilde();
        setup_comb0x2erev_tilde();
        cosine_tilde_setup();
        crackle_tilde_setup();
        crossover_tilde_setup();
        setup_ctl0x2ein();
        setup_ctl0x2eout();
        cusp_tilde_setup();
        decay_tilde_setup();
        decay2_tilde_setup();
        default_setup();
        del_tilde_setup();
        detect_tilde_setup();
        dir_setup();
        dollsym_setup();
        downsample_tilde_setup();
        drive_tilde_setup();
        dust_tilde_setup();
        dust2_tilde_setup();
        else_setup();
        envgen_tilde_setup();
        eq_tilde_setup();
        factor_setup();
        fader_tilde_setup();
        fbdelay_tilde_setup();
        fbsine_tilde_setup();
        fbsine2_tilde_setup();
        f2s_tilde_setup();
        setup_fdn0x2erev_tilde();
        ffdelay_tilde_setup();
        float2bits_setup();
        float2sig_tilde_setup();
        floor_setup();
        floor_tilde_setup();
        fold_setup();
        fold_tilde_setup();
        fontsize_setup();
        format_setup();
        setup_freq0x2eshift_tilde();
        function_setup();
        function_tilde_setup();
        gate2imp_tilde_setup();
        gaussian_tilde_setup();
        gbman_tilde_setup();
        gcd_setup();
        setup_giga0x2erev_tilde();
        glide_tilde_setup();
        glide2_tilde_setup();
        gray_tilde_setup();
        gui_setup();
        henon_tilde_setup();
        highpass_tilde_setup();
        highshelf_tilde_setup();
        hot_setup();
        hz2rad_setup();
        ikeda_tilde_setup();
        imp_tilde_setup();
        imp2_tilde_setup();
        impseq_tilde_setup();
        impulse_tilde_setup();
        impulse2_tilde_setup();
        initmess_setup();
        keyboard_setup();
        lag_tilde_setup();
        lag2_tilde_setup();
        lastvalue_tilde_setup();
        latoocarfian_tilde_setup();
        lb_setup();
        lfnoise_tilde_setup();
        limit_setup();
        lincong_tilde_setup();
        loadbanger_setup();
        logistic_tilde_setup();
        loop_setup();
        lop2_tilde_setup();
        lorenz_tilde_setup();
        lowpass_tilde_setup();
        lowshelf_tilde_setup();
        match_tilde_setup();
        median_tilde_setup();
        merge_setup();
        message_setup();
        messbox_setup();
        metronome_setup();
        midi_setup();
        mouse_setup();
        setup_mov0x2eavg_tilde();
        setup_mov0x2erms_tilde();
        mtx_tilde_setup();
        note_setup();
        setup_note0x2ein();
        setup_note0x2eout();
        noteinfo_setup();
        nyquist_tilde_setup();
        op_tilde_setup();
        openfile_setup();
        oscope_tilde_setup();
        pack2_setup();
        pad_setup();
        pan2_tilde_setup();
        pan4_tilde_setup();
        panic_setup();
        parabolic_tilde_setup();
        peak_tilde_setup();
        setup_pgm0x2ein();
        setup_pgm0x2eout();
        pic_setup();
        pimp_tilde_setup();
        pimpmul_tilde_setup();
        pinknoise_tilde_setup();
        pluck_tilde_setup();
        pmosc_tilde_setup();
        power_tilde_setup();
        properties_setup();
        pulse_tilde_setup();
        pulsecount_tilde_setup();
        pulsediv_tilde_setup();
        quad_tilde_setup();
        quantizer_setup();
        quantizer_tilde_setup();
        rad2hz_setup();
        ramp_tilde_setup();
        rampnoise_tilde_setup();
        setup_rand0x2ef();
        setup_rand0x2ef_tilde();
        s2f_tilde_setup();
#if ENABLE_SFONT
        sfont_tilde_setup();
#endif
        setup_rand0x2ei();
        setup_rand0x2ei_tilde();
        setup_rand0x2eseq();
        randpulse_tilde_setup();
        randpulse2_tilde_setup();
        range_tilde_setup();
        ratio2cents_setup();
        ratio2cents_tilde_setup();
        rec_setup();
        receiver_setup();
        rescale_setup();
        rescale_tilde_setup();
        resonant_tilde_setup();
        resonant2_tilde_setup();
        retrieve_setup();
        rint_setup();
        rint_tilde_setup();
        rms_tilde_setup();
        rotate_tilde_setup();
        routeall_setup();
        router_setup();
        routetype_setup();
        saw_tilde_setup();
        saw2_tilde_setup();
        schmitt_tilde_setup();
        selector_setup();
        separate_setup();
        sequencer_tilde_setup();
        sh_tilde_setup();
        shaper_tilde_setup();
        sig2float_tilde_setup();
        sin_tilde_setup();
        sine_tilde_setup();
        slew_tilde_setup();
        slew2_tilde_setup();
        slice_setup();
        sort_setup();
        spread_setup();
        spread_tilde_setup();
        square_tilde_setup();
        sr_tilde_setup();
        standard_tilde_setup();
        status_tilde_setup();
        stepnoise_tilde_setup();
        susloop_tilde_setup();
        suspedal_setup();
        svfilter_tilde_setup();
        symbol2any_setup();
        table_tilde_setup();
        tabplayer_tilde_setup();
        tabwriter_tilde_setup();
        tempo_tilde_setup();
        setup_timed0x2egate_tilde();
        toggleff_tilde_setup();
        setup_touch0x2ein();
        setup_touch0x2eout();
        tri_tilde_setup();
        setup_trig0x2edelay_tilde();
        setup_trig0x2edelay2_tilde();
        trighold_tilde_setup();
        trunc_setup();
        trunc_tilde_setup();
        unmerge_setup();
        voices_setup();
        vsaw_tilde_setup();
        vu_tilde_setup();
        wt_tilde_setup();
        wavetable_tilde_setup();
        wrap2_setup();
        wrap2_tilde_setup();
        xfade_tilde_setup();
        xgate_tilde_setup();
        xgate2_tilde_setup();
        xmod_tilde_setup();
        xmod2_tilde_setup();
        xselect_tilde_setup();
        xselect2_tilde_setup();
        zerocross_tilde_setup();
        
        
        initialized = 1;
    }
}


