import React from 'react';
import {BaseSettingsControl} from './BaseSettingsControl';
import {Label, Link, Stack, PrimaryButton, Text } from 'office-ui-fabric-react';

export class CustomActionSettingsControl extends BaseSettingsControl {
  colorpickerref:any = null;

  constructor(props:any) {
    super(props);
    this.colorpickerref = null;
    this.state={
      property_values: props.setting,
      name: props.action_name
    }
  }

  componentWillReceiveProps(props: any) {
    this.setState({
      property_values: props.setting,
      name:props.action_name
    });
  }

  public get_value() : any {
    return {value: this.state.property_values.value};
  }

  public render(): JSX.Element {
    return (
      <Stack>
        {
          this.state.property_values.display_name ?
          <Label>{this.state.property_values.display_name}</Label>
          : null
        }
        <Stack horizontal tokens={{childrenGap:5}}>
        {
          this.state.property_values.value ?
            <Text styles ={{
              root: {
                paddingBottom: '0.5em'
              }
            }}>{this.state.property_values.value}</Text>
          : <span/>
        }
        {
          this.state.property_values.help_link_url && this.state.property_values.help_link_text ?
          <Link
            styles = {{
              root: {
                alignSelf:'center',
                paddingBottom: '0.5em'
              }
            }}
            href={ this.state.property_values.help_link_url }
            target='_blank'
          >{ this.state.property_values.help_link_text }</Link> 
          : <span/>
        }
        </Stack>
        <PrimaryButton
            styles={{
              root: {
                alignSelf: 'start'
              }
          }}
          text={this.state.property_values.button_text}
          onClick={()=>this.props.action_callback(this.state.name, this.state.property_values)}
        />
      </Stack>
    );
  }
}
