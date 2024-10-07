/*
 * gfx.h
 */

/*!
 * \file gfx.h
 * \brief Katsu Graphics subsystem
 *
 */

#ifndef __KT_GFX_H__
#define __KT_GFX_H__

#include <katsu/types.h>


/*Graphics Constants*/
#define KT_MAX_TILES				0x4000
#define KT_MAX_SPRITES				2048
#define KT_MAX_TILEMAPS				16
#define KT_MAX_COLORS				2048
#define KT_MAX_PALETTES				(KT_MAX_COLORS >> 4)
#define KT_MAX_MTX					256
#define KT_MAX_LINEMAP_LINES		1024*3


/*! \addtogroup pal_size Palette sizes
 * @{
 */
#define KT_PAL_SIZE_16						0x1		/*!< Palette with 16 entries. */
#define KT_PAL_SIZE_32						0x2		/*!< Palette with 32 entries. */
#define KT_PAL_SIZE_64						0x4		/*!< Palette with 64 entries. */
#define KT_PAL_SIZE_128						0x8		/*!< Palette with 128 entries. */
#define KT_PAL_SIZE_256						0x10	/*!< Palette with 256 entries. */
/*! @} */

/*! \addtogroup layer_id Number of layer
 * @{
 */
#define KT_LAYER0				0
#define KT_LAYER1				1
#define KT_LAYER2				2
#define KT_LAYER3				3
#define KT_LAYER4				4
#define KT_LAYER5				5
#define KT_LAYER6				6
#define KT_LAYER7				7
#define KT_LAYER8				8
#define KT_LAYER9				9
#define KT_LAYER10				10
#define KT_LAYER12				11
#define KT_LAYER13				12
#define KT_LAYER14				13
#define KT_LAYER15				14
#define KT_MAX_LAYERS			16
/*! @} */

/*! \addtogroup layer_type Layer type
 * @{
 */
#define KT_LAYER_NONE			0			/*!< Layer is not drawn. */
#define KT_LAYER_MAP_NORMAL		1			/*!< Draws a normal map on screen. */
#define KT_LAYER_MAP_ROTATION	2			/*!< Draws a map on screen with a 3D matrix applied. */
#define KT_LAYER_SPRITE			3			/*!< Draws a set of sprites. */
/*! @} */


/*! \addtogroup tmap_size Map size
 * @{
 */
#define KT_MAP_SIZE_64x64		0x0
#define KT_MAP_SIZE_128x64		0x1
#define KT_MAP_SIZE_64x128		0x2
#define KT_MAP_SIZE_128x128		0x3
/*! @} */

/* Macros for Sprite init */
#define KT_SPR_POS(x, y)									(((y) << 16) | ((x) & 0xFFFFu))
#define KT_SPR_CHR(tile_num, flip, hsize, vsize, pal)		((((pal & 0x7F)) << 24) | (((vsize) & 0xF) << 20) | (((hsize) & 0xF) << 16) | (((flip) & 0x3) << 14) | ((tile_num) & 0x3FFF))
#define KT_SPR_HUE(hue, hue_alpha)							((((hue) & 0x7FFFu) << 16) | (((hue_alpha) & 0xFFu) << 8))
#define KT_SPR_BLEND(alpha)								((0x80000000u) | ((alpha) & 0xFFu))
#define KT_SPR_MTX(mtx_idx)								((mtx_idx) & 0xFFu)


/*! \addtogroup flip Tile flipping mode
 * @{
 */
#define KT_FLIP_NONE		0x0			/*!< No flipping applied. */
#define KT_FLIP_X			0x1			/*!< Mirrors along X axis. */
#define KT_FLIP_Y			0x2			/*!< Mirrors along Y axis. */
#define KT_FLIP_XY			0x3			/*!< Same as (FLIP_X | FLIP_Y). */
/*! @} */


/*! \addtogroup spr_size Sprite size
 * @{
 */
#define KT_SIZE_8			0x0
#define KT_SIZE_16			0x1
#define KT_SIZE_24			0x2
#define KT_SIZE_32			0x3
#define KT_SIZE_40			0x4
#define KT_SIZE_48			0x5
#define KT_SIZE_56			0x6
#define KT_SIZE_64			0x7
#define KT_SIZE_72			0x8
#define KT_SIZE_80			0x9
#define KT_SIZE_88			0xA
#define KT_SIZE_96			0xB
#define KT_SIZE_104			0xC
#define KT_SIZE_112			0xD
#define KT_SIZE_120			0xE
#define KT_SIZE_128			0xF
/*! @} */


/*! \addtogroup win_id Window IDs
 * @{
 */
#define 	KT_WIN0			0x1
#define 	KT_WIN1			0x2
#define 	KT_WINSPR		0x4
#define 	KT_WINOUT		0x8
#define 	KT_MAX_WIN		4
/*! @} */

/*! \addtogroup win_type Window types
 * @{
 */
#define KT_WIN_TYPE_BOX		0	/*!< Window is defined by a rectangle. */
#define KT_WIN_TYPE_LINE	1	/*!< Window is defined by a set of horizontal lines. */
/*! @} */

#define KT_COLORLINE_FILL_STRETCH		0
#define KT_COLORLINE_FILL_REPEAT		1
#define KT_COLORLINE_FILL_NONE			2
#define KT_COLORLINE_FILL_DUMMY			3
#define KT_MAX_COLORLINE_FILL			4

/*! \addtogroup blnd_modes Blending modes
 * @{
 */
#define KT_BL_FUNC_ADD						0x0		/*!< source color + destination color. */
#define KT_BL_FUNC_SUB						0x1		/*!< source color - destination color. */
/*! @} */

/*! \addtogroup blnd_val Blending values
 * @{
 */
#define KT_BL_ONE						0x0		/*!< Color is multiplied by constant 1. */
#define KT_BL_ZERO						0x1		/*!< Color is multiplied by constant 0. */
#define KT_BL_SRC_ALPHA					0x2		/*!< Color is multiplied by source alpha. */
#define KT_BL_INV_SRC_ALPHA				0x3		/*!< Color is multiplied by 1 - source alpha. */
#define KT_BL_DST_ALPHA					0x4		/*!< Color is multiplied by destination alpha. */
#define KT_BL_INV_DST_ALPHA				0x5		/*!< Color is multiplied by 1 - destination alpha. */
/*! @} */

#define KT_MTX_IDENTITY						0


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*== Structs ==*/

/*! \typedef struct KTChr_t KTChr
 * \brief Struct containing information on a tilemap character. It includes the
 * color palette, the tile's flip mode and the tile ID.
 *
 */
typedef struct KTChr_t {
	u8 pal;			/*!< Palette number. */
	u8 _padding;	/*!< Unused, set to zero. */
	u8 fthi;		/*!< Flip mode and high 6 bits of tile ID. */
	u8 tlo;			/*!< Low 8 bits of tile ID. */
} KTChr;


/*! \typedef struct KTSpr_t KTSpr
 * \brief Struct containing information on a sprite.
 *
 * \details This structure contains settings that specify how a sprite will be drawn on screen. The internal values
 * can be changed directly by using the SPR_POS, SPR_CHR, SPR_HUE, SPR_BLEND and SPR_MAT macros. the
 * SPR_HUE and SPR_BLEND macros can be OR'ed to add these special effets to the <tt>sfx</tt> member.
 *
 * \code
 * pos = [pos_y : 16][pos_x : 16]
 * chr = [- : 1][pal : 7][vsize : 4][hsize : 4][vf : 1][hf : 1][tid : 14]
 * sfx = [trs_act : 1][hue : 15][hue_alpha : 8][alpha : 8]
 * mtx = [-: 24][mtx_idx : 8]
 * \endcode
 */
typedef struct KTSpr_t {
	u32 pos;	/*!< Sprite position on screen. */
	u32 chr;	/*!< Sprite tile, flip mode, size and palette. */
	u32 sfx;	/*!< Special effects, Hue and final color blending. */
	u32 mtx;	/*!< Index of affine matrix applied. */
} KTSpr;


/*! \typedef struct KTLineMapEntry_t KTLineMapEntry
 * \brief Structure used to define line map user data entries.
 *
 * \code
 * ofs_delta = [- : 2][ofs_y_delta_int : 10][ofs_x_delta : 20]
 * scale_x_delta = [- : 6][ofs_y_delta_frac : 10][scale_x_delta : 16]
 * \endcode
 */
typedef struct KTLineMapEntry_t {
	u32 ofs_delta;		/*!< Change in value of the map offset (uses 10.6 fixed point). */
	u32	scale_x_delta;	/*!< Change in value of the horizontal scale (uses 10.6 fixed point). */
} KTLineMapEntry;


/*! \typedef struct KTColor_t KTColor
 * \brief Structure used to define and pass colors to Katsu functions.
 */
typedef struct KTColor_t {
	u8 r;	/*!< Red color component. */
	u8 g;	/*!< Green color component. */
	u8 b;	/*!< Blue color component. */
	u8 a;	/*!< Alpha component. This component is commonly ignored. */
} KTColor;



/* ============================================================================
 * Graphics Loading Functions
 * ============================================================================
 */

/*!
 * \fn void kt_TilesetLoad(u32 tile_id, u32 tile_count, const void* data)
 * \brief Used to load a set of tiles to Tile Memory.
 * \details This function copies the tiles found in the application to Tile Memory, so if the application modifies
 * those tiles they must be reloaded for the changes to be shown.
 *
 * \param[in] tile_id ID of the first tile in Tile Memoy to start the copy.
 * \param[in] tile_count Number of tiles to be copied to Tile Memory.
 * \param[in] data Pointer to the tile data to be loaded. If NULL is passed, the specified tiles are zeroed out.
 */
void kt_TilesetLoad(u32 tile_id, u32 tile_count, const void* data);

/*!
 * \fn void kt_TilemapLoad(u32 tmap, u32 map_size, u32 x, u32 y, u32 w, u32 h, u32 stride, const void* data)
 * \brief Used to load a block of characters to Tilemaps.
 * \details This function copies the characeters found in the application to a tilemap, so if the application modifies
 * those characters they must be reloaded for the changes to be shown. If a map layer uses more than one tilemap you
 * can use the same map size on this function to include the other tilemaps.
 *
 * \param[in] tmap The first tilemap number (other tilemaps can be modified if you use a map_size other than KT_MAP_SIZE_64x64).
 * \param[in] map_size \ref tmap_size.
 * \param[in] x The starting characters x position, where @f$ 0 <= x < Horizontal Map Size @f$.
 * \param[in] y The starting characters y position, where @f$ 0 <= y < Vertical Map Size @f$.
 * \param[in] w Width of character block to copy.
 * \param[in] h Height of character block to copy.
 * \param[in] stride Width of one row of the pointer data.
 * \param[in] data Pointer to the character data to be loaded. If NULL is passed, the specified characters are zeroed.
 */
void kt_TilemapLoad(u32 tmap, u32 map_size, u32 x, u32 y, u32 w, u32 h, u32 stride, const void* data);


/*!
 * \fn kt_TilemapSetChr(u32 tmap, u32 x, u32 y, u32 tile_id, u32 flip, u32 pal)
 * \brief Used to set a single character in a tilemap.
 * \details Note that the (x, y) position of the tilemap (as well as the tilemap number) is wrapped.
 *
 * \param[in] tmap The tilemap number.
 * \param[in] x The characters x position, where @f$ 0 <= x < 64 @f$.
 * \param[in] y The characters y position, where @f$ 0 <= y < 64 @f$.
 * \param[in] tile_id The character's tile ID (Remember that the layer can offset this value).
 * \param[in] flip \ref flip.
 * \param[in] pal Number of the palette used.
 */
void kt_TilemapSetChr(u32 tmap, u32 x, u32 y, u32 tile_id, u32 flip, u32 pal);


/*!
 * \fn void kt_PaletteLoad(u32 pal, u32 pal_size, const void* data)
 * \brief Used to load a set of 16 color palettes to Color Memory.
 * \details This function copies 16-entry palettes from the application to Color Memory. If the application modifies
 * loaded palettes, they must be reloaded for the changes to be shown.
 *
 * \param[in] pal Starting palette number, should be between 0 and KT_MAX_PALETTES-1.
 * \param[in] pal_size \ref pal_size.
 * \param[in] data Pointer to the palette data to be loaded. If NULL is passed, the specified palettes are zeroed out.
 */
void kt_PaletteLoad(u32 pal, u32 pal_size, const void* data);


/*!
 * \fn void kt_PaletteSetColor(u32 color_num, KTColor color)
 * \brief Used to modify a single color in Color Memory.
 *
 * \param[in] color_num The number of the color in Color Memoy that will be modified, should be between 0 and 2048.
 * \param[in] color The new color.
 */
void kt_PaletteSetColor(u32 color_num, KTColor color);


/* ============================================================================
 * Layer Functions
 * ============================================================================
 */

/*!
 * \fn void kt_LayerInitMap(u32 layer, u32 type, u32 tmap, u32 map_size)
 * \brief Initializes the specified layer as a map layer.
 * \details This function is an auxilary function and can be replicated with other layer
 * modifying functions.
 *
 * \param[in] layer \ref layer_id.
 * \param[in] type \ref layer_type (only map layers are expected).
 * \param[in] tmap Number of the first tilemap.
 * \param[in] map_size \ref tmap_size.
 */
void kt_LayerInitMap(u32 layer, u32 type, u32 tmap, u32 map_size);

/*!
 * \fn void kt_LayerInitSprite(u32 layer, u32 spr_count, KTSpr *data)
 * \brief Initializes the specified layer as a sprite layer.
 * \details This function is an auxilary function and can be replicated with other layer
 * modifying functions. This funtion only copies the address of the array of sprites
 * to the layer, this means that the sprite memory must not be freed while it is still
 * attached to the layer.
 *
 * \param[in] layer \ref layer_id.
 * \param[in] spr_count Number of sprites in data.
 * \param[in] data A pointer to the sprite array.
 */
void kt_LayerInitSprite(u32 layer, u32 spr_count, KTSpr *data);

/*!
 * \fn void kt_LayerSetType(u32 layer, u32 type)
 * \brief This function only sets the layer's type.
 * \details Unless the type is between different map types, then one should use
 * the \ref kt_LayerClear function to clear all of the layer's settings.
 *
 * \param[in] layer \ref layer_id.
 * \param[in] type \ref layer_type.
 */
void kt_LayerSetType(u32 layer, u32 type);

/*!
 * \fn void kt_LayerSetMapSize(u32 layer, u32 tmap, u32 map_size)
 * \brief This function sets the tilemap and size of map layers.
 *
 * \param[in] layer \ref layer_id.
 * \param[in] tmap Number of the first tilemap.
 * \param[in] map_size \ref tmap_size.
 */
void kt_LayerSetMapSize(u32 layer, u32 tmap, u32 map_size);

/*!
 * \fn void kt_LayerSetMapOffset(u32 layer, u32 x_ofs, u32 y_ofs)
 * \brief This function sets the position offset of a map layer in integer units.
 * \details The range of the offset is different depending on the size of
 * the map, for example, for 64x64 maps both offsets go from 0 to 512.
 *
 * \param[in] layer \ref layer_id.
 * \param[in] x_ofs Horizontal map offset.
 * \param[in] y_ofs Vertical map offset.
 */
void kt_LayerSetMapOffset(u32 layer, u32 x_ofs, u32 y_ofs);

/*!
 * \fn void kt_LayerSetMapOffsetf(u32 layer, u32 x_ofs, u32 y_ofs, u32 frac)
 * \brief This function sets the position offset of a map layer in fractional units.
 * \details The range of the offset is different depending on the size of
 * the map, for example, for 64x64 maps both offsets go from 0 to 512.
 *
 * \param[in] layer \ref layer_id.
 * \param[in] x_ofs Horizontal map offset.
 * \param[in] y_ofs Vertical map offset.
 * \param[in] frac number of fractional bits in the offset values (between 0 and 10).
 */
void kt_LayerSetMapOffsetf(u32 layer, u32 x_ofs, u32 y_ofs, u32 frac);

/*!
 * \fn void kt_LayerSetMapScale(u32 layer, f32 x_scale, f32 y_scale)
 * \brief This function sets the scaling of a map layer.
 *
 * \param[in] layer \ref layer_id.
 * \param[in] x_scale Horizontal map scaling.
 * \param[in] y_scale Vertical map scaling.
 */
void kt_LayerSetMapScale(u32 layer, f32 x_scale, f32 y_scale);

/*!
 * \fn void kt_LayerSetMapBlend(u32 layer, u32 active, u8 alpha)
 * \brief This function sets the transparency value of a map layer.
 * \details The blending function used for the layer can be set by using
 * kt_LayerSetBlendMode.
 *
 * \param[in] layer \ref layer_id.
 * \param[in] active Wether to do blending or not.
 * \param[in] alpha The blending value.
 */
void kt_LayerSetMapBlend(u32 layer, u32 active, u8 alpha);

/*!
 * \fn void kt_LayerSetMapRect(u32 layer, u32 x, u32 y, u32 w, u32 h)
 * \brief Defines a rectangle on the screen where the map layer will be drawn.
 * \details The function both the width and the height should be values
 * greater than zero, note that the (x, y) values are not signed.
 *
 * \param[in] layer \ref layer_id.
 * \param[in] x X Position of the top left corner of the rectangle.
 * \param[in] y Y Position of the top left corner of the rectangle.
 * \param[in] w The rectangle's width.
 * \param[in] h The rectangle's height.
 */
void kt_LayerSetMapRect(u32 layer, u32 x, u32 y, u32 w, u32 h);

/*!
 * \fn void kt_LayerSetMapMosaic(u32 layer, u32 active, u32 mos_x, u32 mos_y)
 * \brief Set the map layer's mosaic effect.
 * \details The mosaic effect allows for further pixelation in map layers.
 *
 * \param[in] layer \ref layer_id.
 * \param[in] active Wether the effect is active or not.
 * \param[in] mos_x Size of horizontal mosaic.
 * \param[in] mos_y Size of vertical mosaic.
 */
void kt_LayerSetMapMosaic(u32 layer, u32 active, u32 mos_x, u32 mos_y);

/*!
 * \fn void kt_LayerSetMapChrOffset(u32 layer, u32 tile_ofs, u32 pal_ofs)
 * \brief Sets all character's tile and palette offset for map layers.
 * \details For map layers the tile ID and the palette used for each character can be
 * offset by a constant value by using this funciton.
 *
 * \param[in] layer \ref layer_id.
 * \param[in] tile_ofs Value to be added to all character's tile IDs.
 * \param[in] pal_ofs Value to be added to all character's palette numbers.
 */
void kt_LayerSetMapChrOffset(u32 layer, u32 tile_ofs, u32 pal_ofs);

/*!
 * \fn void kt_LayerSetBlendMode(u32 layer, u32 src_factor, u32 dst_factor, u32 func)
 * \brief Sets the layer's transparency blending mode (only for blending active).
 * \details For map layers the alpha values can be set with \ref kt_LayerSetMapBlend, for
 * sprite layers, each sprite will have thier own alpha values. The funciton does the following:
 *
 * \code final_color = src_color * <src_factor> <func> dst_color * <dst_factor> \endcode
 *
 * \param[in] layer \ref layer_id.
 * \param[in] src_factor \ref blnd_val.
 * \param[in] dst_factor \ref blnd_val.
 * \param[in] func \ref blnd_modes.
 */
void kt_LayerSetBlendMode(u32 layer, u32 src_factor, u32 dst_factor, u32 func);


/*!
 * \fn void kt_LayerSetUserData(u32 layer, u32 num_elems, void* data)
 * \brief Sets a layer's user data.
 * \details The layer's user data is a pointer to memory allocated by the user
 * , the use of the data depends on the type of the layer.
 * This funtion only copies the address of the data array
 * to the layer, this means that the user must not free the memory while it is still
 * attached to the layer.
 *
 * \param[in] layer \ref layer_id.
 * \param[in] num_elems The number of elements that the data has.
 * \param[in] data Pointer to the data.
 */
void kt_LayerSetUserData(u32 layer, u32 num_elems, void* data);

/*!
 * \fn void kt_LayerSetWindow(u32 layer, u32 act_windows)
 * \brief Sets a layer's active windows.
 * \details When the pixel of a layer is inside a window that is inactive,
 * then that pixel won't be drawn, else the pixel will be drawn normally.
 *
 * \param[in] layer \ref layer_id.
 * \param[in] act_windows \ref win_id (can be OR'ed).
 */
void kt_LayerSetWindow(u32 layer, u32 act_windows);

/*!
 * \fn void kt_LayerClear(u32 layer)
 * \brief Resets the layer to startup settings, type is set to KT_LAYER_NONE.
 *
 * \param[in] layer \ref layer_id.
 */
void kt_LayerClear(u32 layer);

/*!
 * \fn void kt_LayerClear(u32 layer)
 * \brief Calls \ref kt_LayerClear on all layers.
 */
void kt_LayerClearAll(void);


/* ============================================================================
 * Matrix Functions
 * ============================================================================
 */
/*!
 * \fn void kt_MtxSet(u32 mtx_idx, f32 a, f32 b, f32 c, f32 d)
 * \brief Sets the values of a given matrix in Matrix Memory.
 *
 * \param[in] mtx_idx Index of matrix to set (should be non-zero)
 * \param[in] a
 * \param[in] b
 * \param[in] c
 * \param[in] d
 */
void kt_MtxSet(u32 mtx_idx, f32 a, f32 b, f32 c, f32 d);

/*!
 * \fn void kt_MtxSetRotoscale(u32 mtx_idx, f32 x_scale, f32 y_scale, f32 angle)
 * \brief Sets a rotation and a scale on a given matrix in Matrix Memory.
 *
 * \param[in] mtx_idx Index of matrix to set (should be non-zero)
 * \param[in] x_scale Horizontal scaling factor.
 * \param[in] y_scale Vertical scaling factor.
 * \param[in] angle Angle of rotation in radians.
 */
void kt_MtxSetRotoscale(u32 mtx_idx, f32 x_scale, f32 y_scale, f32 angle);


/* ============================================================================
 * Window Functions
 * ============================================================================
 */
// void kt_WindowBox(u32 win, u32 x, u32 y, u32 w, u32 h);
// void kt_WindowLine(u32 win, u32 fill_mode, u32 line_count, const void* data);


/* ============================================================================
 * Color Related Functions
 * ============================================================================
 */

/*!
 * \fn void kt_BackColor(KTColor color)
 * \brief Sets the back color.
 *
 * \param[in] color Color that will be set.
 */
void kt_BackColor(KTColor color);

/*!
 * \fn void kt_OffsetColor(s32 r, s32 g, s32 b)
 * \brief Sets the offset color.
 *
 * \details All color components must be in the range of -256 to 255. Values larger or lower
 * will be clamped to the maximum and minimum values.
 *
 * \param[in] r Signed red color component that will be added to the final image.
 * \param[in] g Signed green color component that will be added to the final image.
 * \param[in] b Signed blue color component that will be added to the final image.
 */
void kt_OffsetColor(s32 r, s32 g, s32 b);
// void kt_ColorLineLoad(u32 line_count, const void* data);
// void kt_ColorLineSetParams(u32 fill_mode, u32 line_offset);


/* ============================================================================
 * Misc. Functions
 * ============================================================================
 */

/*!
 * \fn void kt_Reset(void)
 * \brief Resets all Katsu graphics memory to match settings on startup.
 */
void kt_Reset(void);
// u32  kt_LerpColor(KTColor c0, KTColor c1, u8 blend);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /*__KT_GFX_H__*/
