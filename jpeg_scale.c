void jpegsw_scale(unsigned char *dest_buf, unsigned char *yptr,
                  unsigned char *uptr, unsigned char *vptr,unsigned int src_width,
                  unsigned int src_height,unsigned int dest_width,unsigned int dest_height)
{
  int hor;
  int ver;
  unsigned int src_x_idx;
  unsigned int src_x_dis;
  unsigned int src_y_idx;
  unsigned int src_y_dis;
  unsigned int left_val;
  unsigned int right_val;
  unsigned char *src_lines_ptr;
  unsigned char *src_upleft_ptr;
  unsigned char *out_ptr;

  for(ver = 0; ver < dest_height; ver++)
  {
    src_y_idx = (src_height*ver)/dest_height;
    src_y_dis = (src_height*ver)%dest_height;

    src_lines_ptr = yptr + src_y_idx*src_width;
    out_ptr = dest_buf + ver*dest_width*3;

    for(hor = 0; hor < dest_width; hor++)
    {
      src_x_idx = (src_width*hor)/dest_width;
      src_x_dis = (src_width*hor)%dest_width;
      src_upleft_ptr = src_lines_ptr + src_x_idx;

      left_val = (src_upleft_ptr[0]*(dest_height - src_y_dis) + src_upleft_ptr[src_width]*src_y_dis)/dest_height;
      right_val = (src_upleft_ptr[1]*(dest_height - src_y_dis) + src_upleft_ptr[src_width+1]*src_y_dis)/dest_height;
      *out_ptr = (left_val*(dest_width - src_x_dis) + right_val*src_x_dis)/dest_width;
      out_ptr += 3;
    }
  }

  for(ver = 0; ver < dest_height; ver++)
  {
    src_y_idx = (src_height*ver)/dest_height;
    src_y_dis = (src_height*ver)%dest_height;

    src_lines_ptr = uptr + src_y_idx*src_width;
    out_ptr = dest_buf + ver*dest_width*3+1;

    for(hor = 0; hor < dest_width; hor++)
    {
      src_x_idx = (src_width*hor)/dest_width;
      src_x_dis = (src_width*hor)%dest_width;
      src_upleft_ptr = src_lines_ptr + src_x_idx;

      left_val = (src_upleft_ptr[0]*(dest_height - src_y_dis) + src_upleft_ptr[src_width]*src_y_dis)/dest_height;
      right_val = (src_upleft_ptr[1]*(dest_height - src_y_dis) + src_upleft_ptr[src_width+1]*src_y_dis)/dest_height;
      *out_ptr = (left_val*(dest_width - src_x_dis) + right_val*src_x_dis)/dest_width;
      out_ptr += 3;
    }
  }

  for(ver = 0; ver < dest_height; ver++)
  {
    src_y_idx = (src_height*ver)/dest_height;
    src_y_dis = (src_height*ver)%dest_height;

    src_lines_ptr = vptr + src_y_idx*src_width;
    out_ptr = dest_buf + ver*dest_width*3+2;

    for(hor = 0; hor < dest_width; hor++)
    {
      src_x_idx = (src_width*hor)/dest_width;
      src_x_dis = (src_width*hor)%dest_width;
      src_upleft_ptr = src_lines_ptr + src_x_idx;

      left_val = (src_upleft_ptr[0]*(dest_height - src_y_dis) + src_upleft_ptr[src_width]*src_y_dis)/dest_height;
      right_val = (src_upleft_ptr[1]*(dest_height - src_y_dis) + src_upleft_ptr[src_width+1]*src_y_dis)/dest_height;
      *out_ptr = (left_val*(dest_width - src_x_dis) + right_val*src_x_dis)/dest_width;
      out_ptr += 3;
    }
  }
}


