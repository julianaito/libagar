package body agar.gui.widget is

  use type c.int;

  function size_allocate
    (widget : widget_access_t;
     alloc  : size_alloc_t) return c.int;
  pragma import (c, size_allocate, "AG_WidgetSizeAlloc");

  function size_allocate
    (widget : widget_access_t;
     alloc  : size_alloc_t) return boolean is
  begin
    return size_allocate (widget, alloc) = 0;
  end size_allocate;

  function enabled (widget : widget_access_t) return c.int;
  pragma import (c, enabled, "agar_widget_enabled");

  function enabled (widget : widget_access_t) return boolean is
  begin
    return enabled (widget) = 1;
  end enabled;

  function disabled (widget : widget_access_t) return c.int;
  pragma import (c, disabled, "agar_widget_disabled");

  function disabled (widget : widget_access_t) return boolean is
  begin
    return disabled (widget) = 1;
  end disabled;

  function focused (widget : widget_access_t) return c.int;
  pragma import (c, focused, "agar_widget_focused");

  function focused (widget : widget_access_t) return boolean is
  begin
    return focused (widget) = 1;
  end focused;

  procedure blit
    (widget  : widget_access_t;
     surface : agar.gui.surface.surface_access_t;
     x       : c.int;
     y       : c.int);
  pragma import (c, blit, "AG_WidgetBlit");

  procedure blit
    (widget  : widget_access_t;
     surface : agar.gui.surface.surface_access_t;
     x       : natural;
     y       : natural) is
  begin
    blit
      (widget  => widget,
       surface => surface,
       x       => c.int (x),
       y       => c.int (y));
  end blit;
 
  procedure blit_from
    (dest_widget : widget_access_t;
     src_widget  : widget_access_t;
     surface_id  : surface_id_t;
     rect        : agar.gui.rect.rect_access_t;
     x           : c.int;
     y           : c.int);
  pragma import (c, blit_from, "AG_WidgetBlitFrom");

  procedure blit_from
    (dest_widget : widget_access_t;
     src_widget  : widget_access_t;
     surface_id  : surface_id_t;
     rect        : agar.gui.rect.rect_access_t;
     x           : integer;
     y           : integer) is
  begin
    blit_from
      (dest_widget => dest_widget,
       src_widget  => src_widget,
       surface_id  => surface_id,
       rect        => rect,
       x           => c.int (x),
       y           => c.int (y));
  end blit_from;

  procedure blit_surface
    (widget     : widget_access_t;
     surface_id : surface_id_t;
     x          : c.int;
     y          : c.int);
  pragma import (c, blit_surface, "agar_widget_blit_surface");

  procedure blit_surface
    (widget     : widget_access_t;
     surface_id : surface_id_t;
     x          : integer;
     y          : integer) is
  begin
    blit_surface
      (widget     => widget,
       surface_id => surface_id,
       x          => c.int (x),
       y          => c.int (y));
  end blit_surface;

  procedure put_pixel32
    (widget : widget_access_t;
     x      : c.int;
     y      : c.int;
     color  : agar.core.types.uint32_t);
  pragma import (c, put_pixel32, "agar_widget_put_pixel32");

  procedure put_pixel32
    (widget : widget_access_t;
     x      : natural;
     y      : natural;
     color  : agar.core.types.uint32_t) is
  begin
    put_pixel32
      (widget => widget,
       x      => c.int (x),
       y      => c.int (y),
       color  => color);
  end put_pixel32;

  procedure put_pixel32_or_clip
    (widget : widget_access_t;
     x      : c.int;
     y      : c.int;
     color  : agar.core.types.uint32_t);
  pragma import (c, put_pixel32_or_clip, "agar_widget_put_pixel32_or_clip");

  procedure put_pixel32_or_clip
    (widget : widget_access_t;
     x      : natural;
     y      : natural;
     color  : agar.core.types.uint32_t) is
  begin
    put_pixel32_or_clip
      (widget => widget,
       x      => c.int (x),
       y      => c.int (y),
       color  => color);
  end put_pixel32_or_clip;

  procedure put_pixel_rgb
    (widget : widget_access_t;
     x      : c.int;
     y      : c.int;
     r      : agar.core.types.uint8_t;
     g      : agar.core.types.uint8_t;
     b      : agar.core.types.uint8_t); 
  pragma import (c, put_pixel_rgb, "agar_widget_put_pixel_rgb");

  procedure put_pixel_rgb
    (widget : widget_access_t;
     x      : natural;
     y      : natural;
     r      : agar.core.types.uint8_t;
     g      : agar.core.types.uint8_t;
     b      : agar.core.types.uint8_t) is
  begin
    put_pixel_rgb
      (widget => widget,
       x      => c.int (x),
       y      => c.int (y),
       r      => r, 
       g      => g,
       b      => b);
  end put_pixel_rgb;

  procedure put_pixel_rgb_or_clip
    (widget : widget_access_t;
     x      : c.int;
     y      : c.int;
     r      : agar.core.types.uint8_t;
     g      : agar.core.types.uint8_t;
     b      : agar.core.types.uint8_t); 
  pragma import (c, put_pixel_rgb_or_clip, "agar_widget_put_pixel_rgb_or_clip");

  procedure put_pixel_rgb_or_clip
    (widget : widget_access_t;
     x      : natural;
     y      : natural;
     r      : agar.core.types.uint8_t;
     g      : agar.core.types.uint8_t;
     b      : agar.core.types.uint8_t) is
  begin
    put_pixel_rgb_or_clip
      (widget => widget,
       x      => c.int (x),
       y      => c.int (y),
       r      => r, 
       g      => g,
       b      => b);
  end put_pixel_rgb_or_clip;

  procedure blend_pixel_rgba
    (widget : widget_access_t;
     x      : c.int;
     y      : c.int;
     color  : color_t;
     func   : agar.gui.colors.blend_func_t);
  pragma import (c, blend_pixel_rgba, "AG_WidgetBlendPixelRGBA");

  procedure blend_pixel_rgba
    (widget : widget_access_t;
     x      : natural;
     y      : natural;
     color  : color_t;
     func   : agar.gui.colors.blend_func_t) is
  begin
    blend_pixel_rgba
      (widget => widget,
       x      => c.int (x),
       y      => c.int (y),
       color  => color,
       func   => func);
  end blend_pixel_rgba;

  procedure blend_pixel_32
    (widget : widget_access_t;
     x      : c.int;
     y      : c.int;
     color  : agar.core.types.uint32_t;
     func   : agar.gui.colors.blend_func_t);
  pragma import (c, blend_pixel_32, "agar_widget_blend_pixel_32");

  procedure blend_pixel_32
    (widget : widget_access_t;
     x      : natural;
     y      : natural;
     color  : agar.core.types.uint32_t;
     func   : agar.gui.colors.blend_func_t) is
  begin
    blend_pixel_32
      (widget => widget,
       x      => c.int (x),
       y      => c.int (y),
       color  => color,
       func   => func);
  end blend_pixel_32;

  function find_point
    (class_mask : cs.chars_ptr;
     x          : c.int;
     y          : c.int) return widget_access_t;
  pragma import (c, find_point, "AG_WidgetFindPoint");

  function find_point
    (class_mask : string;
     x          : natural;
     y          : natural) return widget_access_t
  is
    ca_mask : aliased c.char_array := c.to_c (class_mask);
  begin
    return find_point
      (class_mask => cs.to_chars_ptr (ca_mask'unchecked_access),
       x          => c.int (x),
       y          => c.int (y));
  end find_point;

  function find_rect
    (class_mask : cs.chars_ptr;
     x          : c.int;
     y          : c.int;
     w          : c.int;
     h          : c.int) return widget_access_t;
  pragma import (c, find_rect, "AG_WidgetFindRect");

  function find_rect
    (class_mask : string;
     x          : natural;
     y          : natural;
     w          : positive;
     h          : positive) return widget_access_t
  is
    ca_mask : aliased c.char_array := c.to_c (class_mask);
  begin
    return find_rect
      (class_mask => cs.to_chars_ptr (ca_mask'unchecked_access),
       x          => c.int (x),
       y          => c.int (y),
       w          => c.int (w),
       h          => c.int (h));
  end find_rect;

  function absolute_coords_inside
    (widget : widget_access_t;
     x      : c.int;
     y      : c.int) return c.int;
  pragma import (c, absolute_coords_inside, "agar_widget_area");

  function absolute_coords_inside
    (widget : widget_access_t;
     x      : natural;
     y      : natural) return boolean is
  begin
    return absolute_coords_inside
      (widget => widget,
       x      => c.int (x),
       y      => c.int (y)) = 1;
  end absolute_coords_inside;

  function relative_coords_inside
    (widget : widget_access_t;
     x      : c.int;
     y      : c.int) return c.int;
  pragma import (c, relative_coords_inside, "agar_widget_relative_area");

  function relative_coords_inside
    (widget : widget_access_t;
     x      : natural;
     y      : natural) return boolean is
  begin
    return relative_coords_inside
      (widget => widget,
       x      => c.int (x),
       y      => c.int (y)) = 1;
  end relative_coords_inside;

  --

  procedure set_position
    (widget : widget_access_t;
     x      : natural;
     y      : natural) is
  begin
    widget.x := c.int (x);
    widget.y := c.int (y);
  end set_position;

  procedure modify_position
    (widget : widget_access_t;
     x      : integer := 0;
     y      : integer := 0) is
  begin
    widget.x := widget.x + c.int (x);
    widget.y := widget.y + c.int (y);
  end modify_position;

  procedure set_size
    (widget : widget_access_t;
     width  : positive;
     height : positive) is
  begin
    widget.w := c.int (width);
    widget.h := c.int (height);
  end set_size;

  procedure modify_size
    (widget : widget_access_t;
     width  : integer := 0;
     height : integer := 0) is
  begin
    widget.w := widget.w + c.int (width);
    widget.h := widget.h + c.int (height);
  end modify_size;

  function object (widget : widget_access_t) return agar.core.object.object_access_t is
  begin
    return widget.object'access;
  end object;

end agar.gui.widget;
