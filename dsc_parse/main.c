
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct dsc_rc_range_parameters {
	unsigned char range_min_qp;
	unsigned char range_max_qp;
	signed char range_bpg_offset; /* only 6 bit signal variable */
};

struct dsc_rc_parameter_set {
	unsigned int rc_model_size;
	unsigned char rc_edge_factor;
	unsigned char rc_quant_incr_limit0;
	unsigned char rc_quant_incr_limit1;
	unsigned char rc_tgt_offset_hi;
	unsigned char rc_tgt_offset_lo;
	unsigned char rc_buf_thresh[14]; /* config value need note >> 6 */
	struct dsc_rc_range_parameters rc_range_parameters[15];
};

struct dsc_pps_data_s {
	unsigned char dsc_version_major;
	unsigned char dsc_version_minor;
	unsigned char pps_identifier;
	unsigned char bits_per_component;
	unsigned char line_buf_depth;
	unsigned char block_pred_enable;
	unsigned char convert_rgb;
	unsigned char simple_422;
	unsigned char vbr_enable;
	unsigned int bits_per_pixel;
	unsigned int pic_height;
	unsigned int pic_width;
	unsigned int slice_height;
	unsigned int slice_width;
	unsigned int chunk_size;
	unsigned int initial_xmit_delay;
	unsigned int initial_dec_delay;
	unsigned char initial_scale_value;
	unsigned int scale_increment_interval;
	unsigned int scale_decrement_interval;
	unsigned char first_line_bpg_offset;
	unsigned int nfl_bpg_offset;
	unsigned int slice_bpg_offset;
	unsigned int initial_offset;
	unsigned int final_offset;
	unsigned char flatness_min_qp;
	unsigned char flatness_max_qp;
	struct dsc_rc_parameter_set rc_parameter_set;
	unsigned char native_420;
	unsigned char native_422;
	unsigned char second_line_bpg_offset;
	unsigned int nsl_bpg_offset;
	unsigned int second_line_offset_adj;
	unsigned int hc_active_bytes;
};

struct dsc_pps_data_s dsc_pps_data;

void dump_dsc_pps_info(void)
{
	int i;
	unsigned char buf[512] = {0};
	int k = 0;

	printf("------Picture Parameter Set Start------\n");
	printf("dsc_version_major:%d\n", dsc_pps_data.dsc_version_major);
	printf("dsc_version_minor:%d\n", dsc_pps_data.dsc_version_minor);
	printf("pps_identifier:%d\n", dsc_pps_data.pps_identifier);
	printf("bits_per_component:%u\n", dsc_pps_data.bits_per_component);
	printf("line_buf_depth:%d\n", dsc_pps_data.line_buf_depth);
	printf("block_pred_enable:%d\n", dsc_pps_data.block_pred_enable);
	printf("convert_rgb:%d\n",  dsc_pps_data.convert_rgb);
	printf("simple_422:%d\n", dsc_pps_data.simple_422);
	printf("vbr_enable:%d\n", dsc_pps_data.vbr_enable);
	printf("bits_per_pixel:%u\n", dsc_pps_data.bits_per_pixel);
	printf("pic_height:%d\n", dsc_pps_data.pic_height);
	printf("pic_width:%d\n", dsc_pps_data.pic_width);
	printf("slice_height:%d\n", dsc_pps_data.slice_height);
	printf("slice_width:%d\n", dsc_pps_data.slice_width);
	printf("chunk_size:%d\n", dsc_pps_data.chunk_size);
	printf("initial_xmit_delay:%d\n", dsc_pps_data.initial_xmit_delay);
	printf("initial_dec_delay:%d\n", dsc_pps_data.initial_dec_delay);
	printf("initial_scale_value:%d\n", dsc_pps_data.initial_scale_value);
	printf("scale_increment_interval:%d\n", dsc_pps_data.scale_increment_interval);
	printf("scale_decrement_interval:%d\n", dsc_pps_data.scale_decrement_interval);
	printf("first_line_bpg_offset:%d\n", dsc_pps_data.first_line_bpg_offset);
	printf("nfl_bpg_offset:%d\n", dsc_pps_data.nfl_bpg_offset);
	printf("slice_bpg_offset:%d\n", dsc_pps_data.slice_bpg_offset);
	printf("initial_offset:%d\n", dsc_pps_data.initial_offset);
	printf("final_offset:%d\n", dsc_pps_data.final_offset);
	printf("flatness_min_qp:%d\n", dsc_pps_data.flatness_min_qp);
	printf("flatness_max_qp:%d\n", dsc_pps_data.flatness_max_qp);
	printf("rc_model_size:%d\n", dsc_pps_data.rc_parameter_set.rc_model_size);
	printf("rc_edge_factor:%d\n", dsc_pps_data.rc_parameter_set.rc_edge_factor);
	printf("rc_quant_incr_limit0:%d\n",
		dsc_pps_data.rc_parameter_set.rc_quant_incr_limit0);
	printf("rc_quant_incr_limit1:%d\n",
		dsc_pps_data.rc_parameter_set.rc_quant_incr_limit1);
	printf("rc_tgt_offset_hi:%d\n", dsc_pps_data.rc_parameter_set.rc_tgt_offset_hi);
	printf("rc_tgt_offset_lo:%d\n", dsc_pps_data.rc_parameter_set.rc_tgt_offset_lo);
	printf("*******rc_buf_thresh*******\n");
	for (i = 0; i < 14; i++)
		k += sprintf(buf + k, "%d ",
		dsc_pps_data.rc_parameter_set.rc_buf_thresh[i]);
	printf("%s\n", buf);
	k = 0;
	printf("*******rc_range_parameters*******\n");
	printf("*******range_min_qp*******\n");
	for (i = 0; i < 15; i++)
		k += sprintf(buf + k, "%d ",
		dsc_pps_data.rc_parameter_set.rc_range_parameters[i].range_min_qp);
	printf("%s\n", buf);
	k = 0;
	printf("*******range_max_qp*******\n");
	for (i = 0; i < 15; i++)
		k += sprintf(buf + k, "%d ",
		dsc_pps_data.rc_parameter_set.rc_range_parameters[i].range_max_qp);
	printf("%s\n", buf);
	k = 0;
	printf("*******range_bpg_offset*******\n");
	for (i = 0; i < 15; i++)
		k += sprintf(buf + k, "%d ",
		dsc_pps_data.rc_parameter_set.rc_range_parameters[i].range_bpg_offset);
	printf("%s\n", buf);
	k = 0;
	printf("native_420:%d\n", dsc_pps_data.native_420);
	printf("native_422:%d\n", dsc_pps_data.native_422);
	printf("second_line_bpg_offset:%d\n", dsc_pps_data.second_line_bpg_offset);
	printf("nsl_bpg_offset:%d\n", dsc_pps_data.nsl_bpg_offset);
	printf("second_line_offset_adj:%d\n", dsc_pps_data.second_line_offset_adj);
	printf("------Picture Parameter Set End------\n");
}

void parse_dsc_pps_data(unsigned char *buff)
{
	int i;

	dsc_pps_data.dsc_version_major = (buff[0] >> 4) & 0xf;
	dsc_pps_data.dsc_version_minor = buff[0] & 0xf;
	dsc_pps_data.pps_identifier = buff[1];
	dsc_pps_data.bits_per_component = (buff[3] >> 4) & 0xf;
	printf("%d\n", dsc_pps_data.bits_per_component);
	dsc_pps_data.line_buf_depth = buff[3] & 0xf;
	dsc_pps_data.block_pred_enable = (buff[4] & 0x20) >> 5;
	dsc_pps_data.convert_rgb = (buff[4] & 0x10) >> 4;
	dsc_pps_data.simple_422 = buff[4] & 0x8;
	dsc_pps_data.vbr_enable = buff[4] & 0x4;
	dsc_pps_data.bits_per_pixel = ((buff[4] & 0x3) << 8) | buff[5];
	printf("0x%x\n", (unsigned char)buff[5]);
	dsc_pps_data.pic_height = (buff[6] << 8) | buff[7];
	dsc_pps_data.pic_width = (buff[8] << 8) | buff[9];
	dsc_pps_data.slice_height = (buff[10] << 8) | buff[11];
	dsc_pps_data.slice_width = (buff[12] << 8) | buff[13];
	dsc_pps_data.chunk_size = (buff[14] << 8) | buff[15];
	dsc_pps_data.initial_xmit_delay = ((buff[16] & 0x3) << 8) | buff[17];
	dsc_pps_data.initial_dec_delay = (buff[18] << 8) | buff[19];
	dsc_pps_data.initial_scale_value = buff[21] & 0x3f;
	dsc_pps_data.scale_increment_interval = (buff[22] << 8) | buff[23];
	dsc_pps_data.scale_decrement_interval = ((buff[24] & 0xf) << 8) | buff[25];
	dsc_pps_data.first_line_bpg_offset = buff[27] & 0x1f;
	dsc_pps_data.nfl_bpg_offset = (buff[28] << 8) | buff[29];
	dsc_pps_data.slice_bpg_offset = (buff[30] << 8) | buff[31];
	dsc_pps_data.initial_offset = (buff[32] << 8) | buff[33];
	dsc_pps_data.final_offset = (buff[34] << 8) | buff[35];
	dsc_pps_data.flatness_min_qp = buff[36] & 0x1f;
	dsc_pps_data.flatness_max_qp = buff[37] & 0x1f;
	dsc_pps_data.rc_parameter_set.rc_model_size = (buff[38] << 8) | buff[39];
	dsc_pps_data.rc_parameter_set.rc_edge_factor = buff[40] & 0xf;
	dsc_pps_data.rc_parameter_set.rc_quant_incr_limit0 = buff[41] & 0x1f;
	dsc_pps_data.rc_parameter_set.rc_quant_incr_limit1 = buff[42] & 0x1f;
	dsc_pps_data.rc_parameter_set.rc_tgt_offset_hi = (buff[43] >> 4) & 0xf;
	dsc_pps_data.rc_parameter_set.rc_tgt_offset_lo = buff[43] & 0xf;
	memcpy(dsc_pps_data.rc_parameter_set.rc_buf_thresh, buff + 44, 14);
	for (i = 0; i < 15; i++) {
		dsc_pps_data.rc_parameter_set.rc_range_parameters[i].range_min_qp =
			(buff[58 + 2 * i] & 0xf8) >> 3;
		dsc_pps_data.rc_parameter_set.rc_range_parameters[i].range_max_qp =
			(buff[59 + 2 * i] >> 6) | (((buff[58 + 2 * i] & 0x7)) << 2);
		dsc_pps_data.rc_parameter_set.rc_range_parameters[i].range_bpg_offset =
			buff[59 + 2 * i] & 0x3f;
	}
	dsc_pps_data.native_420 = (buff[88] & 0x2) >> 1;
	dsc_pps_data.native_422 = buff[88] & 0x1;
	dsc_pps_data.second_line_bpg_offset = buff[89] & 0x1f;
	dsc_pps_data.nsl_bpg_offset = (buff[90] << 8) | buff[91];
	dsc_pps_data.second_line_offset_adj = (buff[92] << 8) | buff[93];
}

int my_dsc_read(char *fname)
{
	FILE *fp;
	if ((fp = fopen(fname, "rb")) == NULL)
	{
		fprintf(stderr, "Error: Cannot read DPX file %s\n", fname); 
		perror("Cannot read DPX file");
		return -1;
	}
	char *buffer;
    long offset = 4; // 偏移量
    size_t bytesRead;
    size_t toRead = 128; // 要读取的字节数
    buffer = (unsigned char *)malloc(toRead);
	fseek(fp, offset, SEEK_SET);
	fread(buffer, 1, toRead, fp);
	parse_dsc_pps_data(buffer);
	dump_dsc_pps_info();
	free(buffer);
    fclose(fp);
}

int main()
{
	char *inputFile = "input.dsc";
	my_dsc_read(inputFile);
	return 0;
}
