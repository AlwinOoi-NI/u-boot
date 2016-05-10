/*
 * (C) Copyright 2007 Semihalf
 *
 * Written by: Rafal Jaworowski <raj@semihalf.com>
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 *
 */

#ifndef _API_PRIVATE_H_
#define _API_PRIVATE_H_

void	api_init(void);
void	platform_set_mr(struct sys_info *, unsigned long, unsigned long, int);
int	platform_sys_info(struct sys_info *);

void	dev_enum_reset(void);
int	dev_enum_storage(struct device_info *);
int	dev_enum_net(struct device_info *);

int	dev_open_stor(void *);
int	dev_open_net(void *);
int	dev_close_stor(void *);
int	dev_close_net(void *);

lbasize_t	dev_read_stor(void *, void *, lbasize_t, lbastart_t);
int		dev_read_net(void *, void *, int);
int		dev_write_net(void *, void *, int);

void dev_stor_init(void);

int display_get_info(int type, struct display_info *di);
int display_draw_bitmap(ulong bitmap, int x, int y);
void display_clear(void);

void env_save(void);
void env_perm_set(const char *name, const char *value);

int ubi_part(char *part_name, const char *vid_header_offset);
int ubi_volume_read(char *volume, char *buf, size_t size);
int ubi_volume_write(char *volume, char *buf, size_t size);

int fpga_load(int devnum, const void *buf, size_t bsize);

int run_command(const char *cmd, int flag);

int i2c_read(u8 dev, uint addr, int alen, u8 *data, int length);
int i2c_write(u8 dev, uint addr, int alen, u8 *data, int length);

#endif /* _API_PRIVATE_H_ */
