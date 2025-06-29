//
// Created by malo on 27/05/25.
//

#include "effect.h"
#include <gtk/gtk.h>
#include "view.h"
#include "../controller/controller.h"
#include "../controller/light_wizard.h"
#include "../controller/live_controller.h"


/**
 * @brief Converts a color from HSV color space to RGB color space.
 *
 * This function takes hue (h), saturation (s), and value/brightness (v)
 * and outputs the corresponding red (r), green (g), and blue (b) components.
 *
 * @param h Hue, in the range [0, 1].
 * @param s Saturation, in the range [0, 1].
 * @param v Value (brightness), in the range [0, 1].
 * @param r Pointer to the resulting red component (output, range [0, 1]).
 * @param g Pointer to the resulting green component (output, range [0, 1]).
 * @param b Pointer to the resulting blue component (output, range [0, 1]).
 */
void hsv_to_rgb(double h, double s, double v, double *r, double *g, double *b) {
    int i;
    double f, p, q, t;

    if (s == 0) {
        *r = *g = *b = v;
        return;
    }

    h *= 6;
    i = (int)h;
    f = h - i;
    p = v * (1 - s);
    q = v * (1 - s * f);
    t = v * (1 - s * (1 - f));

    switch (i % 6) {
        case 0: *r = v; *g = t; *b = p; break;
        case 1: *r = q; *g = v; *b = p; break;
        case 2: *r = p; *g = v; *b = t; break;
        case 3: *r = p; *g = q; *b = v; break;
        case 4: *r = t; *g = p; *b = v; break;
        case 5: *r = v; *g = p; *b = q; break;
    }
}



/**
 * @brief Periodically updates the border color of GTK buttons with a rainbow effect.
 *
 * This function is designed to be used with a GTK timer (e.g., g_timeout_add()).
 * It updates the button border color by cycling through hues in the HSV color space.
 *
 * @param user_data Pointer to user data (unused).
 * @return TRUE to continue the timeout function; FALSE to stop it.
 */
gboolean update_border_color(gpointer user_data) {
    static int hue = 0;

    // Convert hue to RGB
    double r, g, b;
    gdouble h = hue / 360.0;
    hsv_to_rgb(h, 1.0, 1.0, &r, &g, &b);

    gchar *css = g_strdup_printf(
    "button { border: 2px solid rgb(%d,%d,%d); }",
    (int)(r * 255), (int)(g * 255), (int)(b * 255)
);

    GtkCssProvider *provider = gtk_css_provider_new();
    gtk_css_provider_load_from_data(provider, css, -1, NULL);

    GdkDisplay *display = gdk_display_get_default();
    GdkScreen *screen = gdk_display_get_default_screen(display);

    gtk_style_context_add_provider_for_screen(screen,
        GTK_STYLE_PROVIDER(provider),
        GTK_STYLE_PROVIDER_PRIORITY_USER);

    g_free(css);
    g_object_unref(provider);

    hue = (hue + 20) % 360;  // Cycle color
    return G_SOURCE_CONTINUE;  // Keep looping
}






/**
 * @brief Applies a custom CSS stylesheet to the GTK interface.
 *
 * Loads and applies a CSS stylesheet defined by the macro CSS_PATH.
 * This is used to customize the appearance of GTK widgets in the application.
 */
void apply_css(void) {
    GtkCssProvider *provider = gtk_css_provider_new();
    GdkDisplay *display = gdk_display_get_default();
    GdkScreen *screen = gdk_display_get_default_screen(display);

    gtk_css_provider_load_from_path(provider, CSS_PATH, NULL);
    gtk_style_context_add_provider_for_screen(screen,
        GTK_STYLE_PROVIDER(provider),
        GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

    g_object_unref(provider);
}