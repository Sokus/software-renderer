#include "base.h"
#include "summoned_platform.h"
#include "summoned_math.h"
#include "summoned_intrinsics.h"

#include "summoned.h"

global U32 global_crc32_lookup_table[256] = {
    0x00000000,0x77073096,0xEE0E612C,0x990951BA,0x076DC419,0x706AF48F,0xE963A535,0x9E6495A3,0x0EDB8832,0x79DCB8A4,0xE0D5E91E,0x97D2D988,0x09B64C2B,0x7EB17CBD,0xE7B82D07,0x90BF1D91,
    0x1DB71064,0x6AB020F2,0xF3B97148,0x84BE41DE,0x1ADAD47D,0x6DDDE4EB,0xF4D4B551,0x83D385C7,0x136C9856,0x646BA8C0,0xFD62F97A,0x8A65C9EC,0x14015C4F,0x63066CD9,0xFA0F3D63,0x8D080DF5,
    0x3B6E20C8,0x4C69105E,0xD56041E4,0xA2677172,0x3C03E4D1,0x4B04D447,0xD20D85FD,0xA50AB56B,0x35B5A8FA,0x42B2986C,0xDBBBC9D6,0xACBCF940,0x32D86CE3,0x45DF5C75,0xDCD60DCF,0xABD13D59,
    0x26D930AC,0x51DE003A,0xC8D75180,0xBFD06116,0x21B4F4B5,0x56B3C423,0xCFBA9599,0xB8BDA50F,0x2802B89E,0x5F058808,0xC60CD9B2,0xB10BE924,0x2F6F7C87,0x58684C11,0xC1611DAB,0xB6662D3D,
    0x76DC4190,0x01DB7106,0x98D220BC,0xEFD5102A,0x71B18589,0x06B6B51F,0x9FBFE4A5,0xE8B8D433,0x7807C9A2,0x0F00F934,0x9609A88E,0xE10E9818,0x7F6A0DBB,0x086D3D2D,0x91646C97,0xE6635C01,
    0x6B6B51F4,0x1C6C6162,0x856530D8,0xF262004E,0x6C0695ED,0x1B01A57B,0x8208F4C1,0xF50FC457,0x65B0D9C6,0x12B7E950,0x8BBEB8EA,0xFCB9887C,0x62DD1DDF,0x15DA2D49,0x8CD37CF3,0xFBD44C65,
    0x4DB26158,0x3AB551CE,0xA3BC0074,0xD4BB30E2,0x4ADFA541,0x3DD895D7,0xA4D1C46D,0xD3D6F4FB,0x4369E96A,0x346ED9FC,0xAD678846,0xDA60B8D0,0x44042D73,0x33031DE5,0xAA0A4C5F,0xDD0D7CC9,
    0x5005713C,0x270241AA,0xBE0B1010,0xC90C2086,0x5768B525,0x206F85B3,0xB966D409,0xCE61E49F,0x5EDEF90E,0x29D9C998,0xB0D09822,0xC7D7A8B4,0x59B33D17,0x2EB40D81,0xB7BD5C3B,0xC0BA6CAD,
    0xEDB88320,0x9ABFB3B6,0x03B6E20C,0x74B1D29A,0xEAD54739,0x9DD277AF,0x04DB2615,0x73DC1683,0xE3630B12,0x94643B84,0x0D6D6A3E,0x7A6A5AA8,0xE40ECF0B,0x9309FF9D,0x0A00AE27,0x7D079EB1,
    0xF00F9344,0x8708A3D2,0x1E01F268,0x6906C2FE,0xF762575D,0x806567CB,0x196C3671,0x6E6B06E7,0xFED41B76,0x89D32BE0,0x10DA7A5A,0x67DD4ACC,0xF9B9DF6F,0x8EBEEFF9,0x17B7BE43,0x60B08ED5,
    0xD6D6A3E8,0xA1D1937E,0x38D8C2C4,0x4FDFF252,0xD1BB67F1,0xA6BC5767,0x3FB506DD,0x48B2364B,0xD80D2BDA,0xAF0A1B4C,0x36034AF6,0x41047A60,0xDF60EFC3,0xA867DF55,0x316E8EEF,0x4669BE79,
    0xCB61B38C,0xBC66831A,0x256FD2A0,0x5268E236,0xCC0C7795,0xBB0B4703,0x220216B9,0x5505262F,0xC5BA3BBE,0xB2BD0B28,0x2BB45A92,0x5CB36A04,0xC2D7FFA7,0xB5D0CF31,0x2CD99E8B,0x5BDEAE1D,
    0x9B64C2B0,0xEC63F226,0x756AA39C,0x026D930A,0x9C0906A9,0xEB0E363F,0x72076785,0x05005713,0x95BF4A82,0xE2B87A14,0x7BB12BAE,0x0CB61B38,0x92D28E9B,0xE5D5BE0D,0x7CDCEFB7,0x0BDBDF21,
    0x86D3D2D4,0xF1D4E242,0x68DDB3F8,0x1FDA836E,0x81BE16CD,0xF6B9265B,0x6FB077E1,0x18B74777,0x88085AE6,0xFF0F6A70,0x66063BCA,0x11010B5C,0x8F659EFF,0xF862AE69,0x616BFFD3,0x166CCF45,
    0xA00AE278,0xD70DD2EE,0x4E048354,0x3903B3C2,0xA7672661,0xD06016F7,0x4969474D,0x3E6E77DB,0xAED16A4A,0xD9D65ADC,0x40DF0B66,0x37D83BF0,0xA9BCAE53,0xDEBB9EC5,0x47B2CF7F,0x30B5FFE9,
    0xBDBDF21C,0xCABAC28A,0x53B39330,0x24B4A3A6,0xBAD03605,0xCDD70693,0x54DE5729,0x23D967BF,0xB3667A2E,0xC4614AB8,0x5D681B02,0x2A6F2B94,0xB40BBE37,0xC30C8EA1,0x5A05DF1B,0x2D02EF8D,
};

uint HashData(void* data_p, size_t data_size, uint seed)
{
    U32 crc = (U32)~seed;
    U8* data = (U8 *)data_p;
    U32* crc32_lut = global_crc32_lookup_table;
    while (data_size-- != 0)
        crc = (crc >> 8) ^ crc32_lut[(crc & 0xFF) ^ *data++];
    return (uint)(~crc);
}

uint HashStr(char* data_p, size_t data_size, uint seed)
{
    U32 crc = (U32)~seed;
    U8* data = (U8 *)data_p;
    U32* crc32_lut = global_crc32_lookup_table;
    if (data_size != 0)
    {
        while (data_size-- != 0)
        {
            U8 c = *data++;
            crc = (crc >> 8) ^ crc32_lut[(crc & 0xFF) ^ c];
        }
    }
    else
    {
        U8 c = *data;
        while (c != 0)
        {
            crc = (crc >> 8) ^ crc32_lut[(crc & 0xFF) ^ c];
            c = *(++data);
        }
    }
    return (uint)(~crc);
}

uint HashPtr(void* ptr, uint seed)
{
    U32 crc = (U32)~seed;
    U32* crc32_lut = global_crc32_lookup_table;
    crc = (crc >> 8) ^ crc32_lut[(crc & 0xFF) ^ *((U8 *)ptr)];
    return (uint)(~crc);
}

void
DrawRectangle(OffscreenBuffer *buffer,
              F32 real_min_x, F32 real_min_y, F32 real_max_x, F32 real_max_y,
              F32 r, F32 g, F32 b, F32 a)
{
    if(a > 0.0f)
    {
        int min_x = (int)RoundF32(real_min_x);
        int min_y = (int)RoundF32(real_min_y);
        int max_x = (int)RoundF32(real_max_x);
        int max_y = (int)RoundF32(real_max_y);
        
        int screen_width = buffer->width;
        int screen_height = buffer->height;
        
        int screen_min_x = CLAMP_BOT(min_x, 0);
        int screen_min_y = CLAMP_BOT(screen_height - max_y - 1, 0);
        int screen_max_x = CLAMP_TOP(max_x, screen_width);
        int screen_max_y = CLAMP_TOP(screen_height - min_y - 1, screen_height);
        
        U8 *dest_row = (U8 *)(buffer->memory
                              + screen_min_y*buffer->pitch
                              + screen_min_x*buffer->bytes_per_pixel);
        U8 *dest;
        
        for(int y = screen_min_y;
            y < screen_max_y;
            ++y)
        {
            dest = dest_row;
            
            for(int x = screen_min_x;
                x < screen_max_x;
                ++x)
            {
                *dest = (U8)((a*b*255.0f) + ((F32)(*dest)*(1.0f-a)));
                ++dest;
                
                *dest = (U8)((a*g*255.0f) + ((F32)(*dest)*(1.0f-a)));
                ++dest;
                
                *dest = (U8)((a*r*255.0f) + ((F32)(*dest)*(1.0f-a)));
                ++dest;
                
                ++dest;
            }
            
            dest_row += buffer->pitch;
        }
    }
}

void
DrawRectangleExplicit(OffscreenBuffer *buffer,
                      F32 min_x, F32 min_y, F32 max_x, F32 max_y,
                      F32 fill_r, F32 fill_g, F32 fill_b, F32 fill_a,
                      BorderType border_style, F32 border_width,
                      F32 border_r, F32 border_g, F32 border_b, F32 border_a)
{
    F32 width = max_x - min_x;
    F32 height = max_y - min_y;
    
    // TODO(sokus): Figure out if those functions should even work on floats.
    if(width > 0.0f && height > 0.0f)
    {
        F32 inside_border = (border_style == BorderType_Centered ? border_width/2 :
                             border_style == BorderType_Inner ? border_width : 0);
        F32 outside_border = (border_style == BorderType_Centered ? border_width/2 :
                              border_style == BorderType_Outer ? border_width : 0);
        
        if(fill_a > 0.0f
           && width - 2*inside_border > 0.0f
           && height - 2*inside_border > 0.0f)
        {
            DrawRectangle(buffer,
                          min_x+inside_border, min_y+inside_border, max_x-inside_border, max_y-inside_border,
                          fill_r, fill_g, fill_b, fill_a);
        }
        
        if(border_a > 0.0f
           && (inside_border > 0.0f || outside_border > 0.0f))
        {
            DrawRectangle(buffer,
                          min_x-outside_border, min_y-outside_border, max_x+outside_border, min_y+inside_border,
                          border_r, border_g, border_b, border_a);
            DrawRectangle(buffer,
                          min_x-outside_border, min_y+inside_border, min_x+inside_border, max_y-inside_border,
                          border_r, border_g, border_b, border_a);
            DrawRectangle(buffer,
                          min_x-outside_border, max_y-inside_border, max_x+outside_border, max_y+outside_border,
                          border_r, border_g, border_b, border_a);
            DrawRectangle(buffer,
                          max_x-inside_border, min_y+inside_border, max_x+outside_border, max_y-inside_border,
                          border_r, border_g, border_b, border_a);
        }
        
    }
}

void
DrawBitmap(OffscreenBuffer *buffer,
           U32 *bitmap_data, int bitmap_w, int bitmap_h,
           F32 part_x, F32 part_y, F32 part_w, F32 part_h,
           F32 pos_x, F32 pos_y,
           F32 r, F32 g, F32 b, F32 a)
{
    UNUSED(bitmap_h);
    
    bitmap_data = bitmap_data + (int)part_y*(int)bitmap_w + (int)part_x;
    
    int min_x = (int)RoundF32(pos_x);
    int min_y = (int)RoundF32(pos_y);
    int max_x = (int)RoundF32(pos_x + part_w);
    int max_y = (int)RoundF32(pos_y + part_h);;
    
    int source_offset_x = 0;
    if(min_x < 0)
    {
        source_offset_x = -min_x;
        min_x = 0;
    }
    
    int source_offset_y = 0;
    if(min_y < 0)
    {
        source_offset_y = -min_y;
        min_y = 0;;
    }
    
    max_x = CLAMP_TOP(max_x, buffer->width);
    max_y = CLAMP_TOP(max_y, buffer->height);
    
    U32 *source_row = bitmap_data + source_offset_y*(int)bitmap_w + source_offset_x;
    U8 *dest_row = (U8 *)buffer->memory + min_y*buffer->pitch + min_x*buffer->bytes_per_pixel;
    
    for(int y = min_y;
        y < max_y;
        ++y)
    {
        U32 *dest = (U32 *)dest_row;
        U32 *source = source_row;
        
        for(int x = min_x;
            x < max_x;
            ++x)
        {
            F32 new_a = (F32)(*source >> 24) / 255.0f * a;
            
            F32 source_r = (F32)((*source >> 16) & 0xFF) * r;
            F32 source_g = (F32)((*source >> 8) & 0xFF) * g;
            F32 source_b = (F32)((*source >> 0) & 0xFF) * b;
            
            F32 dest_r = (F32)((*dest >> 16) & 0xFF);
            F32 dest_g = (F32)((*dest >> 8) & 0xFF);
            F32 dest_b = (F32)((*dest >> 0) & 0xFF);
            
            F32 new_r = (1.0f-new_a)*dest_r + new_a*source_r;
            F32 new_g = (1.0f-new_a)*dest_g + new_a*source_g;
            F32 new_b = (1.0f-new_a)*dest_b + new_a*source_b;
            
            *dest = (((U32)(new_r + 0.5f) << 16) |
                     ((U32)(new_g + 0.5f) << 8) |
                     ((U32)(new_b + 0.5f) << 0));
            ++dest;
            ++source;
        }
        
        dest_row += buffer->pitch;
        source_row += bitmap_w;
    }
}

void
DrawAsci(OffscreenBuffer *buffer, Font *font,
         U8 character,
         F32 pos_x, F32 pos_y,
         F32 r, F32 g, F32 b, F32 a)
{
    int glyph_pos_x = character % font->glyphs_per_row;
    int glyph_pos_y = character / font->glyphs_per_col;
    
    DrawBitmap(buffer,
               font->data,
               font->w,
               font->h,
               (F32)(glyph_pos_x*font->glyph_w),
               (F32)(glyph_pos_y*font->glyph_h),
               (F32)font->glyph_w,
               (F32)font->glyph_h,
               pos_x, pos_y,
               r, g, b, a);
}

void
DrawString(OffscreenBuffer *buffer, Font *font,
           char *source, size_t source_count,
           F32 pos_x, F32 pos_y,
           F32 r, F32 g, F32 b, F32 a)
{
    for(size_t index = 0;
        index < source_count;
        index++)
    {
        F32 offset_x = (F32)((int)index*font->glyph_w);
        
        DrawAsci(buffer, font, (U8)(source[index]), pos_x+offset_x, pos_y, r, g, b, a);
    }
}


OffscreenBuffer
OffscreenBufferTrim(OffscreenBuffer *screen_buffer, int min_x, int min_y, int max_x, int max_y)
{
    int min_x_clamped = CLAMP(0, min_x, screen_buffer->width);
    int min_y_clamped = CLAMP(0, min_y, screen_buffer->height);
    int max_x_clamped = CLAMP(min_x_clamped, max_x, screen_buffer->width);
    int max_y_clamped = CLAMP(min_y_clamped, max_y, screen_buffer->height);
    
    OffscreenBuffer result = {0};
    result.memory = (void *)((U8 *)screen_buffer->memory
                             + (min_y_clamped*screen_buffer->pitch)
                             + min_x_clamped*screen_buffer->bytes_per_pixel);
    result.width = max_x_clamped - min_x_clamped;
    result.height = max_y_clamped - min_y_clamped;
    result.pitch = screen_buffer->pitch;
    result.bytes_per_pixel = screen_buffer->bytes_per_pixel;
    
    return result;
}

Rect RectRel(Rect parent,
             F32 x0_pct, F32 y0_pct, F32 x1_pct, F32 y1_pct,
             F32 top, F32 left, F32 bottom, F32 right)
{
    F32 parent_width = parent.x1 - parent.x0;
    F32 parent_height = parent.y1 - parent.y0;
    Rect result;
    result.x0 = parent.x0 + x0_pct*parent_width + left;
    result.y0 = parent.y0 + y0_pct*parent_height + bottom;
    result.x1 = parent.x0 + x1_pct*parent_width - right;
    result.y1 = parent.y0 + y1_pct*parent_height - top;
    return result;
}

Rect RectAbs(F32 pos_x, F32 pos_y, F32 width, F32 height)
{
    Rect result;
    result.x0 = pos_x;
    result.y0 = pos_y;
    result.x1 = pos_x + width;
    result.y1 = pos_y + height;
    return result;
}

void String8ListPlaceInRect(String8List *list, Rect rect, int glyph_w, int glyph_h, F32 line_jump, bool start_at_bottom)
{
    F32 current_x = 0;
    F32 current_y = 0;
    
    F32 rect_width = rect.x1 - rect.x0;
    F32 rect_height = rect.y1 - rect.y0;
    
    for(String8Node *node = list->first;
        node != 0;
        node = node->next)
    {
        if(current_x > 0
           && current_x + (F32)((int)node->string.size * glyph_w) > rect_width)
        {
            current_x = 0;
            current_y += line_jump;
        }
        
        node->style.pos_x = current_x;
        node->style.pos_y = current_y;
        
        if(node->style.flags & STYLEFLAG_LINE_END)
        {
            current_x = 0;
            current_y += line_jump;
        }
        else
        {
            current_x += (F32)(((int)node->string.size + 1) * glyph_w);
        }
    }
    
    if(start_at_bottom)
    {
        for(String8Node *node = list->first;
            node != 0;
            node = node->next)
        {
            F32 offset = -current_y - (F32)glyph_h + rect_height;
            node->style.pos_y += offset;
        }
    }
}

void DrawString8List(OffscreenBuffer *buffer, Font *font, String8List *list)
{
    for(String8Node *node = list->first;
        node != 0;
        node = node->next)
    {
        F32 pos_x = node->style.pos_x;
        F32 pos_y = node->style.pos_y;
        
        F32 node_width_in_pixels = (F32)((int)node->string.size * font->glyph_w);
        
        bool fits_in_buffer = (pos_y + (F32)font->glyph_h >= 0
                               && pos_y < (F32)buffer->height
                               && pos_x + node_width_in_pixels >= 0
                               && pos_x < (F32)buffer->width);
        if(fits_in_buffer)
        {
            DrawString(buffer, font,
                       (char *)(node->string.str),
                       node->string.size,
                       pos_x, pos_y,
                       node->style.r, node->style.g, node->style.b, node->style.a);
        }
    }
}

void InputUpdate(Input *input)
{
    MEMORY_COPY(input->keys_down_duration_previous,
                input->keys_down_duration,
                sizeof(input->keys_down_duration));
    
    for(int key_idx = 0;
        key_idx < Input_COUNT;
        ++key_idx)
    {
        F32 old_duration = input->keys_down_duration[key_idx];
        F32 new_duration = (input->keys_down[key_idx] ?
                            (old_duration >= 0.0f ?
                             old_duration + input->time_dt : 0.0f) :
                            -1.0f);
        
        input->keys_down_duration[key_idx] = new_duration;
    }
}

bool IsDown(Input *input, int key_index)
{
    ASSERT(key_index >= 0 && key_index < Input_COUNT);
    bool result = input->keys_down[key_index];
    return result;
}

bool WasDown(Input *input, int key_index)
{
    ASSERT(key_index >= 0 && key_index < Input_COUNT);
    bool result = input->keys_down_duration_previous[key_index] >= 0.0f;
    return result;
}

void FrameStart(UIContext *ui, Input *input)
{
    
}

void GameUpdateAndRender(GameMemory *memory, Input *input, OffscreenBuffer *buffer, FontPack *font_pack)
{
    UNUSED(font_pack);
    
    GameState *game_state = (GameState *)memory->permanent_storage;
    
    if(!(memory->is_initialized))
    {
        game_state->time = 0;
        
        InitializeArena(&game_state->arena,
                        (U8 *)memory->permanent_storage + sizeof(GameState),
                        0);
        
        memory->is_initialized = true;
    }
    
    InputUpdate(input);
    game_state->time += input->time_dt;
    
    MemoryArena transient_arena;
    InitializeArena(&transient_arena,
                    (U8 *)memory->transient_storage,
                    MEGABYTES(1));
    
    
    
    DrawRectangle(buffer, 0, 0, (F32)buffer->width, (F32)buffer->height, 0, 0, 0, 1);
    
    for(int idx = 0;
        idx < Input_COUNT;
        ++idx)
    {
        bool is_down = IsDown(input, idx);
        bool was_down = WasDown(input, idx);
        
        F32 r = !was_down && is_down;
        F32 g = was_down && is_down;
        F32 b = was_down && !is_down;
        DrawRectangle(buffer, (F32)idx*10, 0, (F32)(idx+1)*10, 10, r, g, b, 1);
    }
    
    ++game_state->frame_index;
}
